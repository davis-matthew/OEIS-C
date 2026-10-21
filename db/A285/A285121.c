/*
 * A285121  Min(|d(k+1-i) - d(i)|, for i = 1..k), where d(1),..,d(k) are the
 *          divisors of n*(n+1)*(n+2)/6.
 * https://oeis.org/A285121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Min(|d(k+1-i) - d(i)|, for i = 1..k), where d(1)..d(k)
 *            are the divisors of n*(n+1)*(n+2)/6
 * Range:     u8 n <= 10, u16 n <= 639, u32 n <= 2150, u64 n <= 9998,
 *            u128 n <= 9998, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static int seq_compare(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n < 1, return 0 for safety */

    /* Compute the triangular number T(n) = n*(n+1)*(n+2)/6 */
    seq_t tri = (seq_t)n * ((seq_t)n + 1) * ((seq_t)n + 2) / 6;

    /* m fits in 64 bits for n up to 10⁴ */
    uint64_t m = (uint64_t)tri;

    /* Factor m */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pf_cnt = 0;
    factor_u64(m, primes, exps, &pf_cnt);

    /* Generate all divisors of m */
    const size_t MAX_DIVS = 20000;  /* > divisor count for m ≤ 1e12 */
    seq_t *divs = (seq_t *)malloc(MAX_DIVS * sizeof(seq_t));
    if (!divs) return 0;  /* allocation failure */

    size_t divs_cnt = 1;
    divs[0] = 1;  /* divisor 1 */

    for (size_t f = 0; f < pf_cnt; ++f) {
        uint64_t p = primes[f];
        uint64_t e = exps[f];
        size_t prev_cnt = divs_cnt;
        seq_t p_pow = 1;
        for (uint64_t exp = 1; exp <= e; ++exp) {
            p_pow *= (seq_t)p;                /* p^exp */
            for (size_t j = 0; j < prev_cnt; ++j) {
                if (divs_cnt == MAX_DIVS) {   /* safety check */
                    free(divs);
                    return 0;
                }
                divs[divs_cnt++] = divs[j] * p_pow;
            }
        }
    }

    /* Sort divisors to enable mirrored pairing */
    qsort(divs, divs_cnt, sizeof(seq_t), seq_compare);

    /* Compute the minimum mirrored difference */
    seq_t min_diff = divs[divs_cnt - 1] - divs[0];
    size_t half = divs_cnt / 2;
    for (size_t i = 1; i < half; ++i) {
        seq_t diff = divs[divs_cnt - 1 - i] - divs[i];
        if (diff < min_diff) min_diff = diff;
    }
    if (divs_cnt % 2 == 1) { /* middle element gives diff = 0 */
        if ((seq_t)0 < min_diff) min_diff = 0;
    }

    free(divs);
    return min_diff;
}
