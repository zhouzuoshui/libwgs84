// registration.c
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    double x, y, z;
} Point3D;

// 计算点集质心
void compute_centroid(const Point3D *points, size_t n, gsl_vector *centroid)
{
    double cx = 0.0, cy = 0.0, cz = 0.0;
    for (size_t i = 0; i < n; i++) {
        cx += points[i].x;
        cy += points[i].y;
        cz += points[i].z;
    }
    cx /= n;
    cy /= n;
    cz /= n;

    gsl_vector_set(centroid, 0, cx);
    gsl_vector_set(centroid, 1, cy);
    gsl_vector_set(centroid, 2, cz);
}

// 去质心化，结果存入 3xn 矩阵
void demean_points(const Point3D *points, size_t n, const gsl_vector *centroid, gsl_matrix *centered)
{
    for (size_t i = 0; i < n; i++) {
        gsl_matrix_set(centered, 0, i, points[i].x - gsl_vector_get(centroid, 0));
        gsl_matrix_set(centered, 1, i, points[i].y - gsl_vector_get(centroid, 1));
        gsl_matrix_set(centered, 2, i, points[i].z - gsl_vector_get(centroid, 2));
    }
}

// 执行 Kabsch 算法
int kabsch_rotation(const gsl_matrix *P_centered,  // 3 x n
                    const gsl_matrix *Q_centered,  // 3 x n
                    gsl_matrix *R                  // 输出：3x3 旋转矩阵
)
{
    size_t n = P_centered->size2;  // 点数

    // 1. 计算协方差矩阵 H = P^T * Q
    gsl_matrix *H = gsl_matrix_alloc(3, 3);
    gsl_blas_dgemm(CblasTrans, CblasNoTrans, 1.0, P_centered, Q_centered, 0.0, H);

    // 2. SVD 分解 H = U * S * V^T
    gsl_matrix *U = gsl_matrix_alloc(3, 3);
    gsl_matrix *V = gsl_matrix_alloc(3, 3);
    gsl_vector *S = gsl_vector_alloc(3);
    gsl_vector *work = gsl_vector_alloc(3);

    gsl_matrix_memcpy(U, H);  // SVD 会修改 U

    int status = gsl_linalg_SV_decomp(U, V, S, work);
    if (status != GSL_SUCCESS) {
        fprintf(stderr, "SVD 分解失败\n");
        goto cleanup;
    }

    // 3. R = V * U^T
    // 注意：GSL 的 SVD 返回的是 H = U * S * V^T，所以 V 是右奇异向量
    gsl_matrix *UT = gsl_matrix_alloc(3, 3);
    gsl_matrix_transpose_memcpy(UT, U);

    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, V, UT, 0.0, R);

    // 4. 检查行列式，避免反射（det(R) 应为 +1）
    double det =
        gsl_matrix_get(R, 0, 0)
            * (gsl_matrix_get(R, 1, 1) * gsl_matrix_get(R, 2, 2) - gsl_matrix_get(R, 1, 2) * gsl_matrix_get(R, 2, 1))
        - gsl_matrix_get(R, 0, 1)
              * (gsl_matrix_get(R, 1, 0) * gsl_matrix_get(R, 2, 2) - gsl_matrix_get(R, 1, 2) * gsl_matrix_get(R, 2, 0))
        + gsl_matrix_get(R, 0, 2)
              * (gsl_matrix_get(R, 1, 0) * gsl_matrix_get(R, 2, 1) - gsl_matrix_get(R, 1, 1) * gsl_matrix_get(R, 2, 0));

    if (det < 0) {
        // 修正：将 V 的最后一列取反（即 R 的最后一行取反）
        for (size_t i = 0; i < 3; i++) {
            gsl_matrix_set(R, 2, i, -gsl_matrix_get(R, 2, i));
        }
    }

cleanup:
    gsl_matrix_free(H);
    gsl_matrix_free(U);
    gsl_matrix_free(V);
    gsl_vector_free(S);
    gsl_vector_free(work);
    return status;
}

// 计算平移 t = centroid_Q - R * centroid_P
void compute_translation(const gsl_vector *centroid_P, const gsl_vector *centroid_Q, const gsl_matrix *R, gsl_vector *t)
{
    gsl_vector *R_cp = gsl_vector_alloc(3);
    gsl_blas_dgemv(CblasNoTrans, 1.0, R, centroid_P, 0.0, R_cp);
    gsl_vector_memcpy(t, centroid_Q);
    gsl_vector_sub(t, R_cp);  // t = centroid_Q - R * centroid_P
    gsl_vector_free(R_cp);
}

// 主函数示例
int main()
{
    // 示例数据：3 个点
    const size_t n = 3;
    Point3D P[] = { { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 0.0, 1.0 } };

    Point3D Q[] = {
        { 0.0, 1.0, 0.0 },   // P[0] 绕 z 轴旋转 90°
        { -1.0, 0.0, 0.0 },  // P[1]
        { 0.0, 0.0, 1.0 }    // P[2]
    };

    // 分配 GSL 数据结构
    gsl_vector *centroid_P = gsl_vector_alloc(3);
    gsl_vector *centroid_Q = gsl_vector_alloc(3);
    gsl_matrix *P_centered = gsl_matrix_alloc(3, n);  // 3 x n
    gsl_matrix *Q_centered = gsl_matrix_alloc(3, n);
    gsl_matrix *R = gsl_matrix_alloc(3, 3);
    gsl_vector *t = gsl_vector_alloc(3);

    // 1. 计算质心
    compute_centroid(P, n, centroid_P);
    compute_centroid(Q, n, centroid_Q);

    // 2. 去质心化
    demean_points(P, n, centroid_P, P_centered);
    demean_points(Q, n, centroid_Q, Q_centered);

    // 3. 求解旋转矩阵
    int status = kabsch_rotation(P_centered, Q_centered, R);
    if (status != GSL_SUCCESS) {
        fprintf(stderr, "求解旋转失败\n");
        return 1;
    }

    // 4. 计算平移
    compute_translation(centroid_P, centroid_Q, R, t);

    // 输出结果
    printf("旋转矩阵 R:\n");
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            printf("%8.4f ", gsl_matrix_get(R, i, j));
        }
        printf("\n");
    }

    printf("平移向量 t: [%8.4f, %8.4f, %8.4f]\n", gsl_vector_get(t, 0), gsl_vector_get(t, 1), gsl_vector_get(t, 2));

    // 清理
    gsl_vector_free(centroid_P);
    gsl_vector_free(centroid_Q);
    gsl_matrix_free(P_centered);
    gsl_matrix_free(Q_centered);
    gsl_matrix_free(R);
    gsl_vector_free(t);

    return 0;
}