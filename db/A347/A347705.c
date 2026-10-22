/*
 * A347705  Number of factorizations of n with reverse-alternating product >
 *          1.
 * https://oeis.org/A347705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   rev_alt_gt_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ROEIS A347705: Number of factorizations of n with reverse-alternating product > 1
   a(n) = A001055(n) - A347438(n) */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/rev_alt_gt_one.h"

/* Recursive enumeration of all nondecreasing factorizations of rem.
   start is the smallest factor allowed.  fac holds the current factor
   sequence; depth is its current length.  cnt is the running count of
   factorizations whose reverse-alternating product exceeds 1. */
static void dfs(uint64_t rem, uint64_t start, uint64_t *fac,
                size_t depth, size_t *cnt)
{
    if (rem == 1) {
        if (rev_alt_gt_one(fac, depth))
            ++(*cnt);
        return;
    }

    for (uint64_t f = start; f * f <= rem; ++f) {
        if (rem % f == 0) {
            fac[depth] = f;
            dfs(rem / f, f, fac, depth + 1, cnt);
        }
    }

    /* final factor is the remaining rem itself (>= start) */
    if (rem >= start) {
        fac[depth] = rem;
        dfs(1, rem, fac, depth + 1, cnt);
    }
}

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 0 by definition, a(0) unused */
        return (seq_t)0;

    uint64_t N = (uint64_t)n;
    size_t cnt = 0;
    uint64_t fac[64];    /* enough for all n up to 2^63-1 */

    dfs(N, 2, fac, 0, &cnt);
    return (seq_t)cnt;
}
