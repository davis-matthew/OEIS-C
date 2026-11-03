/*
 * A308706  Number of chiral pairs of set partitions of a primitive cycle of n
 *          elements having exactly two different elements.
 * https://oeis.org/A308706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = ((1/(2*n)) * Sum_{odd d|n} mu(d) * 2^(n/d) * -
 *            Sum_{d|n} mu(n/d) * 2^floor(d/2)) / 2 * = (Sum_{odd d|n} mu(d) *
 *            2^(n/d) * - 2*n * Sum_{d|n} mu(n/d) * 2^floor(d/2)) / (4*n)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 31, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 39 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow2.h"

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

static int compare_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Positive and negative accumulators for S1 and S2 */
    seq_t pos1 = 0, neg1 = 0;   /* S1: Sum_{odd d|n} mu(d) * 2^(n/d)  */
    seq_t pos2 = 0, neg2 = 0;   /* S2: Sum_{d|n} mu(n/d) * 2^floor(d/2) */

    /* Enumerate all divisors d of n */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0)
            continue;
        idx_t d2 = n / d;          /* counterpart divisor */

        /* ----- divisor d ----- */

        /* S1: odd d */
        if (d & 1U) {
            int mu = mu_of(d);
            seq_t term = pow2(n / d);   /* 2^(n/d) */
            if (mu > 0)
                pos1 += term;
            else if (mu < 0)
                neg1 += term;
        }

        /* S2: mu(n/d) * 2^floor(d/2) */
        int mu_nd = mu_of(d2);           /* mu(n/d) */
        seq_t term2 = pow2(d >> 1);      /* 2^floor(d/2) */
        if (mu_nd > 0)
            pos2 += term2;
        else if (mu_nd < 0)
            neg2 += term2;

        if (d2 == d)
            continue;   /* already processed this divisor */

        /* ----- divisor d2 ----- */

        /* S1: odd d2 */
        if (d2 & 1U) {
            int mu = mu_of(d2);
            seq_t term = pow2(d);        /* 2^(n/d2) = 2^d */
            if (mu > 0)
                pos1 += term;
            else if (mu < 0)
                neg1 += term;
        }

        /* S2: mu(d) * 2^floor(d2/2) */
        int mu_d = mu_of(d);             /* mu(d) */
        seq_t term2b = pow2(d2 >> 1);    /* 2^floor(d2/2) */
        if (mu_d > 0)
            pos2 += term2b;
        else if (mu_d < 0)
            neg2 += term2b;
    }

    /* Compute numerator N = (pos1 + 2*n*neg2) - (neg1 + 2*n*pos2) */
    unsigned int two_n = (unsigned int)(2 * n);       /* 2*n fits in unsigned int for n <= 10000 */
    seq_t term_p = pos1 + mul_small(neg2, two_n);     /* pos1 + 2*n*neg2 */
    seq_t term_m = neg1 + mul_small(pos2, two_n);     /* neg1 + 2*n*pos2 */

    seq_t N;
    if (compare_seq_t(&term_p, &term_m) < 0)
        N = 0;
    else
        N = term_p - term_m;

    /* Denominator = 4*n */
    seq_t denom = mul_small((seq_t)n, 4);

    return N / denom;
}
