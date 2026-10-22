/*
 * A327658  Number of factorizations of n that are empty or whose factors have
 *          a common divisor > 1.
 * https://oeis.org/A327658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of unordered factorizations of n where all factors share
   a common divisor > 1 (empty factorization for n = 1).  The function
   accounts for all such unordered factorizations, including the trivial
   factorization n itself, and counts the empty factorization only for n=1. */
#include <stdint.h>
#include "../utils/gcd_uint64.h"

/* Euclidean algorithm for 64‑bit integers (used only for small values). */

/* Depth‑first search: count special factorizations of n.
   Parameters:
     n      - remaining product to factor
     min_f  - smallest factor allowed (to enforce nondecreasing order)
     cur_g  - GCD of factors chosen so far (0 if no factor yet)
*/
static seq_t dfs(uint64_t n, uint64_t min_f, seq_t cur_g) {
    seq_t cnt = 0;

    /* single‑factor case: n itself */
    uint64_t gcd_val = (cur_g == (seq_t)0) ? n : gcd_uint64((uint64_t)cur_g, n);
    if (gcd_val > 1) cnt++;

    /* proper factors (unordered, nondecreasing) */
    for (uint64_t d = min_f; d * d <= n; ++d) {
        if (n % d == 0) {
            uint64_t r = n / d;
            uint64_t new_gcd = gcd_uint64((cur_g == (seq_t)0 ? d : (uint64_t)cur_g), d);
            if (new_gcd == 1) continue;          /* gcd would become 1 – prune */
            cnt += dfs(r, d, (seq_t)new_gcd);
        }
    }
    return cnt;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;       /* empty factorization */
    return dfs((uint64_t)n, (uint64_t)2, (seq_t)0);
}
