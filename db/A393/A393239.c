/*
 * A393239  Numbers k such that A000219(k) == 0 (mod 3).
 * https://oeis.org/A393239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with A000219(k) == 0 (mod 3), where
 *            sum_{k>=0} A000219(k) x^k = Product_{m>=1} 1/(1-x^m)^m, computed
 *            mod 3 using (1-x^m)^(-m) = Product_j (1-x^(m*3^j))^(-d_j) with
 *            d_j the base-3 digits of m
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *zs_list = NULL;
static size_t zs_cnt = 0;
static size_t zs_N = 0;

static void zs_build(size_t N)
{
    unsigned char *F = (unsigned char *)malloc(N + 1);
    size_t k, j;
    free(zs_list);
    zs_list = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    zs_cnt = 0;
    for (j = 0; j <= N; j++) F[j] = 0;
    F[0] = 1;
    for (k = 1; k <= N; k++) {
        size_t m = k, s = k;
        while (m > 0 && s <= N) {
            unsigned d = (unsigned)(m % 3);
            unsigned r;
            for (r = 0; r < d; r++)
                for (j = s; j <= N; j++)
                    F[j] = (unsigned char)((F[j] + F[j - s]) % 3);
            m /= 3;
            s *= 3;
        }
    }
    for (j = 1; j <= N; j++)
        if (F[j] == 0) zs_list[zs_cnt++] = (uint32_t)j;
    free(F);
    zs_N = N;
}

seq_t a(idx_t n)
{
    size_t N = zs_N ? zs_N : 1024;
    if (zs_N == 0 || zs_cnt < (size_t)n) {
        while (1) {
            zs_build(N);
            if (zs_cnt >= (size_t)n) break;
            N *= 2;
        }
    }
    return (seq_t)zs_list[(size_t)n - 1];
}
