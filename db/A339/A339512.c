/*
 * A339512  Number of subsets of {1..n} whose elements have the same number of
 *          distinct prime factors.
 * https://oeis.org/A339512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{ω≥0} (2^{cnt[ω]} – 1) where cnt[ω] counts integers
 *            ≤ n having exactly ω distinct prime factors
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 61, u64 n <= 116,
 *            u128 n <= 225, big any n
 * Helpers:   build_spf, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* External utilities */
#include "../utils/build_spf.h"   /* void build_spf(idx_t limit, idx_t **spf_out); */
#include "../utils/pow_uint.h"    /* seq_t pow_uint(uint64_t base, idx_t exp); */

seq_t a(idx_t n)
{
    /* Base case: empty set */
    if (n == 0)
        return (seq_t)1;

    /* Build array of smallest prime factors up to n. */
    idx_t *spf = NULL;
    build_spf(n, &spf);   /* spf[0..n] will be valid */

    /* Count how many numbers have each ω.  The array size n+1
       is sufficient because ω(k) ≤ log₂(k) < n for k ≥ 2. */
    size_t *cnt = (size_t*)calloc((size_t)(n + 1), sizeof(size_t));
    if (!cnt) {
        free(spf);
        return (seq_t)0;
    }

    /* ω(1) = 0 */
    cnt[0] = 1;

    /* For each k = 2..n compute ω(k) via the spf array. */
    for (idx_t k = 2; k <= n; ++k) {
        idx_t x = k;
        size_t omega = 0;
        while (x > 1) {
            idx_t p = spf[x];
            ++omega;
            while (x % p == 0)
                x /= p;
        }
        ++cnt[omega];
    }

    free(spf);

    /* Sum contributions: one empty subset overall,
       plus, for each ω-class, all non‑empty subsets. */
    seq_t result = (seq_t)1;   /* empty subset */
    for (size_t w = 0; w <= (size_t)n; ++w) {
        size_t c = cnt[w];
        if (c == 0)
            continue;

        seq_t term = pow_uint(2ULL, (idx_t)c); /* 2^c */
        term -= (seq_t)1;                       /* exclude empty subset of this class */
        result += term;
    }

    free(cnt);
    return result;
}
