/*
 * A308992  Sum of the sixth largest parts in the partitions of n into 8
 *          parts.
 * https://oeis.org/A308992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over partitions of n into 8 parts of the 6th largest
 *            part (equivalently the 3rd smallest part). This implementation
 *            uses the identity a(n) = Σ_{m≥1} m * C(n,m), where C(n,m) counts
 *            partitions of n into 8 parts with the 3rd smallest part equal to
 *            m. C(n,m) is computed by summing over po...
 * Range:     u8 n <= 23, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Static tables for partition numbers into at most 5 parts */
static uint64_t *p_leq5 = NULL;          /* partition count into at most 5 parts  */
static uint64_t *p_exact[6] = {NULL};   /* partition count into exactly k parts  (k=0..5) */
static idx_t  precomp_up_to = 0;        /* maximum n that has been pre‑computed */

/* Re‑compute partition tables up to N */
static void recompute_partitions(idx_t N)
{
    if (N <= precomp_up_to) return;

    /* allocate new tables */
    size_t sz = (size_t)N + 1;
    for (int k = 0; k <= 5; ++k) {
        if (p_exact[k]) free(p_exact[k]);
        p_exact[k] = (uint64_t *)malloc(sz * sizeof(uint64_t));
    }
    if (p_leq5) free(p_leq5);
    p_leq5 = (uint64_t *)malloc(sz * sizeof(uint64_t));

    /* base case */
    for (idx_t i = 0; i <= N; ++i) p_exact[0][i] = (i == 0 ? 1 : 0);

    /* partitions into exactly k parts */
    for (int k = 1; k <= 5; ++k) {
        for (idx_t i = 0; i <= N; ++i) {
            uint64_t val = 0;
            if (i >= (idx_t)k) val += p_exact[k][i - (idx_t)k];
            if (i >= 1)           val += p_exact[k - 1][i - 1];
            p_exact[k][i] = val;
        }
    }

    /* partitions into at most 5 parts */
    for (idx_t i = 0; i <= N; ++i) {
        uint64_t sum = 0;
        for (int k = 0; k <= 5; ++k) sum += p_exact[k][i];
        p_leq5[i] = sum;
    }

    precomp_up_to = N;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* offset 0: a(0)=0 */
    if (n < 8) return 0;

    /* pre‑compute partition tables up to n */
    recompute_partitions(n);

    seq_t result = 0;
    idx_t max_m = (n - 2) / 6;            /* m ranges from 1 to floor((n-2)/6) */

    for (idx_t m = 1; m <= max_m; ++m) {
        idx_t max_s = 2 * m;
        for (idx_t s = 2; s <= max_s; ++s) {
            if (s + 6 * m > n) continue;               /* t < 0 */
            idx_t t = n - s - 6 * m;                   /* t >= 0 */

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
            /* C99 or later: cast to 64‑bit unsigned */
            uint64_t t_u64 = (uint64_t)t;
#else
            uint64_t t_u64 = (unsigned long long)t;
#endif

            /* number of pairs (b1,b2) with 1 <= b1 <= b2 <= m and b1 + b2 = s */
            idx_t b1_min = (s > m) ? (s - m) : 1;
            idx_t b1_max = s / 2;
            if (b1_max > m) b1_max = m;
            if (b1_min > b1_max) continue;
            uint64_t pair_cnt = (uint64_t)(b1_max - b1_min + 1);

            /* add contribution: m * pair_cnt * p_leq5[t] */
            seq_t prod = (seq_t)m;
            prod *= (seq_t)pair_cnt;
            prod *= (seq_t)p_leq5[t_u64];
            result += prod;
        }
    }

    return result;
}
