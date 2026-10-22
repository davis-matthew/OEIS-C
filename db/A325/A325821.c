/*
 * A325821  Multiplication table for carryless product i X j in base 3 for i
 *          >= 1 and j >= 1, read by antidiagonals.
 * https://oeis.org/A325821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = carryless_product(i, j), where * i = k, j = t+1-k, * with
 *            t = ceil((sqrt(8*n+1)-1)/2), * k = n - (t-1)*t/2. * The
 *            carryless product is computed as a coefficient‑wise *
 *            convolution of the base‑3 digits of i and j with results *
 *            reduced modulo 3
 * Range:     u8 n <= 639, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stddef.h>

static seq_t carryless_product(idx_t a, idx_t b)
{
    /* Store base‑3 digits of a and b */
    unsigned char da[64], db[64];
    size_t la = 0, lb = 0;
    while (a) { da[la++] = a % 3u; a /= 3; }
    while (b) { db[lb++] = b % 3u; b /= 3; }

    /* Convolution – result digits in dr, reduced modulo 3 */
    unsigned char dr[128];
    for (size_t i = 0; i < sizeof(dr); ++i) dr[i] = 0;
    for (size_t i = 0; i < la; ++i) {
        for (size_t j = 0; j < lb; ++j) {
            unsigned d = (da[i] * db[j]) % 3u;
            unsigned sum = dr[i + j] + d;
            dr[i + j] = sum >= 3u ? sum - 3u : sum;
        }
    }

    /* Convert digits back to a decimal integer */
    seq_t result = 0;
    seq_t pow3 = 1;
    size_t max_len = la + lb;
    for (size_t k = 0; k < max_len; ++k) {
        if (dr[k]) result += pow3 * dr[k];
        pow3 *= 3;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find the antidiagonal index t such that (t-1)*t/2 < n <= t*(t+1)/2 */
    idx_t t = (idx_t)((sqrtl(8.0L * n + 1.0L) - 1.0L) / 2.0L);
    while ((__int128)t * (t + 1) / 2 < n) ++t;

    /* Compute offset inside the antidiagonal */
    idx_t prev = (idx_t)(((__int128)(t - 1) * t) / 2);
    idx_t k = n - prev;            /* 1‑based position within the diagonal */

    idx_t i = k;
    idx_t j = t + 1 - k;

    return carryless_product(i, j);
}
