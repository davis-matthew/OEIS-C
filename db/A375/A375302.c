/*
 * A375302  a(n) is the rank of row n of A375301 in a lexicographic
 *          permutation of [1, ..., n].
 * https://oeis.org/A375302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = rank (0-based) of permutation pi_n in lexicographic order
 *            of S_n, where pi_n(k)+k is a power of 2 for all k (row n of
 *            A375301); a(n) = Sum_{i=1..n} c_i*(n-i)!, c_i = #{j>i : pi_n(j)
 *            < pi_n(i)}
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint64_t A375302_next_pow2(uint64_t x)
{
    uint64_t p = 1;
    while (p <= x) p <<= 1;
    return p;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t m = (size_t)n;
    uint64_t *perm = (uint64_t *)malloc((m + 2) * sizeof(uint64_t));
    unsigned char *used = (unsigned char *)malloc(m + 2);
    seq_t *fact = mem_new(seq_t, (m + 2));
    if (!perm || !used || !fact) {
        if (perm) free(perm);
        if (used) free(used);
        if (fact) mem_free(fact);
        return (seq_t)0;
    }
    for (size_t i = 0; i <= m + 1; ++i) { used[i] = 0; perm[i] = 0; }

    for (size_t i = m; i >= 1; --i) {
        if (used[i]) continue;
        uint64_t p = A375302_next_pow2((uint64_t)i);
        uint64_t k = p - (uint64_t)i;
        perm[i] = k;
        perm[k] = (uint64_t)i;
        used[i] = 1;
        used[k] = 1;
    }

    fact[0] = (seq_t)1;
    for (size_t i = 1; i <= m; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    seq_t rank = (seq_t)0;
    for (size_t i = 1; i <= m; ++i) {
        uint64_t c = 0;
        for (size_t j = i + 1; j <= m; ++j)
            if (perm[j] < perm[i]) ++c;
        rank += (seq_t)c * fact[m - i];
    }

    free(perm);
    free(used);
    mem_free(fact);
    return rank;
}
