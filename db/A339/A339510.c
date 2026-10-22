/*
 * A339510  Number of subsets of {1..n} whose elements have the same smallest
 *          prime factor.
 * https://oeis.org/A339510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1 + Σ_{p=1..n} (2^{m_p} – 1), where m_p is the number of
 * integers k∈[1..n] having smallest prime factor p (m_1 = 1 for k = 1).
 *
 * Implementation: build a sieve for the smallest prime factor up to n,
 * count for each prime p how many numbers ≤ n have that p as smallest prime
 * factor, then sum 2^{m_p} – 1 for each p (plus the empty set).
 */

#include <stdlib.h>
#include <stddef.h>

/* Compute a(n) – see formula above. */
seq_t a(idx_t n)
{
    /* static state – cached sieve */
    static idx_t *spf   = NULL;   /* smallest prime factor for each i */
    static idx_t *cnt   = NULL;   /* count of numbers with spf == p     */
    static idx_t  cap   = 0;      /* current capacity (>= n+1)          */

    /* Grow the cache if necessary. */
    if ((idx_t)(n + 1) > cap) {
        free(spf);
        free(cnt);

        cap = n + 1;
        spf = (idx_t*)calloc((size_t)cap, sizeof(idx_t));
        cnt = (idx_t*)calloc((size_t)cap, sizeof(idx_t));

        /* Build the smallest‑prime‑factor sieve. */
        for (idx_t i = 2; i < cap; ++i) {
            if (spf[i] == 0) {                 /* i is prime   */
                spf[i] = i;
                if (i * i <= n) {              /* avoid overflow */
                    for (idx_t j = i * i; j < cap; j += i) {
                        if (spf[j] == 0) {
                            spf[j] = i;
                        }
                    }
                }
            }
        }
    }

    /* Zero the counters for the current n. */
    for (idx_t i = 0; i <= n; ++i) {
        cnt[i] = 0;
    }

    /* Number 1 has “smallest prime factor” 1. */
    if (n >= 1) {
        cnt[1] = 1;
    }

    /* Count how many integers ≤ n have each prime as their smallest factor. */
    for (idx_t i = 2; i <= n; ++i) {
        idx_t p = spf[i];
        /* p is at most n, so the array index is safe. */
        ++cnt[p];
    }

    /* Sum the contributions: 2^{cnt[p]} – 1 for each p, plus the empty set. */
    seq_t result = (seq_t)1;   /* empty set */

    for (idx_t p = 1; p <= n; ++p) {
        idx_t m = cnt[p];
        if (m == 0) {
            continue;
        }
        seq_t term = (seq_t)1;
        for (idx_t k = 0; k < m; ++k) {
            term = term * (seq_t)2;
        }
        term = term - (seq_t)1;
        result = result + term;
    }

    return result;
}
