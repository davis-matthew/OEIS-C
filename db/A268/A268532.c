/*
 * A268532  Decimal representation of permutations of lengths 1, 2, 3, ...
 *          arranged first by number of inversions and then lexicographically.
 * https://oeis.org/A268532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal representation of the permutation of length k *
 *            that is the n-th permutation when permutations of all lengths
 *            1,2,3,... * are ordered first by number of inversions
 *            (ascending) and then lexicographically. * The algorithm finds
 *            the length k, the inversion sum m, and ranks the permuta...
 * Range:     u8 n <= 7, u16 n <= 153, u32 n <= 5913, u64 n <= 5913,
 *            u128 n <= 5913, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* find k such that sum_{i=1}^{k} i! ≥ n */
    unsigned long long fact = 1;
    idx_t cumulative = 0;
    idx_t k = 0;
    while (1) {
        ++k;
        fact *= (unsigned long long)k;          /* k! fits in 64‑bit up to k = 20 */
        cumulative += fact;
        if (cumulative >= n) break;
    }
    idx_t prev_cum = cumulative - fact;
    idx_t rank_global = n - prev_cum - 1;       /* 0‑based rank within length k */

    /* dynamic programming for Lehmer codes with fixed inversion sum */
    idx_t max_inv = k * (k - 1) / 2;               /* maximum inversions */
    size_t sumlen = (size_t)max_inv + 1;
    size_t table_rows = k + 1;
    size_t table_size = table_rows * sumlen;       /* small, ≤ ~4000 cells */
    unsigned long long *dp = (unsigned long long*)malloc(table_size * sizeof(unsigned long long));
    if (!dp) return 0;                            /* allocation failure */

    /* base: at position k, only sum = 0 is possible */
    for (size_t s = 0; s < sumlen; ++s)
        dp[k * sumlen + s] = (s == 0) ? 1ULL : 0ULL;

    /* fill DP from bottom up */
    for (int pos = k - 1; pos >= 0; --pos) {
        unsigned int limit = (unsigned int)(k - pos - 1);  /* max Lehmer digit at pos */
        for (size_t s = 0; s < sumlen; ++s) {
            unsigned long long ways = 0;
            if (s <= max_inv) {
                unsigned int d;
                for (d = 0; d <= limit && d <= s; ++d) {
                    ways += dp[(pos + 1) * sumlen + (s - d)];
                }
            }
            dp[pos * sumlen + s] = ways;
        }
    }

    /* find inversion sum m and rank within that inversion block */
    idx_t cum = 0;
    idx_t m = 0;
    idx_t rank_within = rank_global;
    for (idx_t sum = 0; sum <= max_inv; ++sum) {
        unsigned long long cnt = dp[0 * sumlen + sum];
        if (rank_within < cnt) {
            m = sum;
            break;
        }
        rank_within -= cnt;
    }

    /* unrank Lehmer code lexicographically among codes summing to m */
    idx_t *lehmer = (idx_t*)malloc(k * sizeof(idx_t));
    if (!lehmer) { free(dp); return 0; }
    idx_t remaining = m;
    idx_t r = rank_within;
    for (size_t pos = 0; pos < (size_t)k; ++pos) {
        unsigned int limit = (unsigned int)(k - pos - 1);
        unsigned int chosen = 0;
        for (unsigned int d = 0; d <= limit; ++d) {
            if (remaining < d) break;
            unsigned long long ways = dp[(pos + 1) * sumlen + (remaining - d)];
            if (r < ways) {
                chosen = d;
                break;
            } else {
                r -= ways;
            }
        }
        lehmer[pos] = chosen;
        remaining -= chosen;
    }

    /* convert Lehmer code to permutation of numbers 1..k */
    idx_t *avail = (idx_t*)malloc(k * sizeof(idx_t));
    idx_t *perm = (idx_t*)malloc(k * sizeof(idx_t));
    if (!avail || !perm) { free(dp); free(lehmer); free(avail); free(perm); return 0; }
    for (size_t i = 0; i < (size_t)k; ++i) avail[i] = (idx_t)(i + 1);
    size_t avail_sz = k;
    for (size_t pos = 0; pos < (size_t)k; ++pos) {
        size_t d = (size_t)lehmer[pos];
        perm[pos] = avail[d];
        /* shift left to remove used element */
        for (size_t j = d; j + 1 < avail_sz; ++j) avail[j] = avail[j + 1];
        --avail_sz;
    }

    /* interpret permutation as decimal digits to form result */
    seq_t result = 0;
    for (size_t i = 0; i < (size_t)k; ++i) {
        result = result * 10 + (seq_t)perm[i];
    }

    free(dp);
    free(lehmer);
    free(avail);
    free(perm);
    return result;
}
