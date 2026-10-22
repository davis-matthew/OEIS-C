/*
 * A301984  a(n) is the greatest positive number k such that the binary digits
 *          of any number in the interval 1..k appear in order but not
 *          necessarily as consecutive digits in the binary representation of
 *          n.
 * https://oeis.org/A301984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * a(n) = maximum k such that for every m ∈ [1..k] the binary
 * representation of m appears as a subsequence (not necessarily
 * contiguous) in the binary representation of n.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/floor_log2_uint64.h"   /* floor_log2_uint64 is provided */

/* Test if m appears as a subsequence of n (both are non‑zero 64‑bit integers). */
static inline bool subseq(idx_t n, idx_t m)
{
    if (m == 0)
        return true;               /* not needed, but keeps logic simple */
    int p_n = floor_log2_uint64((uint64_t)n);
    int p_m = floor_log2_uint64((uint64_t)m);
    int i = p_n, j = p_m;

    while (i >= 0 && j >= 0) {
        if ( ((n >> i) & 1ULL) == ((m >> j) & 1ULL) )
            --j;                    /* matched a bit of m */
        --i;
    }
    return j < 0;                  /* all bits of m matched */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* outside the OEIS offset, harmless */

    seq_t best = 1;                /* at least 1 always satisfies the test */
    for (idx_t k = 1;; ++k) {
        bool ok = true;
        for (idx_t m = 1; m <= k; ++m) {
            if (!subseq(n, m)) {
                ok = false;
                break;
            }
        }
        if (ok)
            best = (seq_t)k;        /* update best with the current k */
        else
            break;                  /* first failure, k is too large */
    }
    return best;
}
