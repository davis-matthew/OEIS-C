/*
 * A294935  Characteristic function for nonabundant numbers (A263837): a(n) =
 *          1 if A001065(n) <= n, 0 otherwise.
 * https://oeis.org/A294935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 1129, u16 n <= 9454, u32 n <= 9454, u64 n <= 9454,
 *            u128 n <= 9454, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A263837: Characteristic function for nonabundant numbers.
   a(n) = 1 if sum of proper divisors of n (A001065(n)) <= n,
       = 0 otherwise.  */

/* This implementation uses an O(sqrt(n)) divisor‑sum algorithm
   that stops early if the sum already exceeds n.  It requires
   the helper function _floor_isqrt_u64 from "../utils/_floor_isqrt_u64.h". */
#include "../utils/_floor_isqrt_u64.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) /* Off‑by‑one safety, a(0) is not used but define reasonably. */
        return 1;

    /* sum of proper divisors (A001065) */
    seq_t sum = 0;
    idx_t lim = _floor_isqrt_u64(n);

    for (idx_t d = 1; d <= lim; ++d) {
        if (n % d == 0) {
            if (d == 1) {
                /* 1 is the only proper divisor that pairs with n itself. */
                if (sum > ((seq_t)n - 1))       /* would exceed n after adding 1 */
                    return 0;
                sum += 1;
            } else {
                /* Add divisor d and its partner q = n/d, unless they are equal. */
                idx_t q = n / d;
                if (sum > ((seq_t)n - (seq_t)d))
                    return 0;
                sum += (seq_t)d;
                if (q != d) {
                    /* q cannot be n because d > 1 implies q < n. */
                    if (sum > ((seq_t)n - (seq_t)q))
                        return 0;
                    sum += (seq_t)q;
                }
            }
        }
    }

    /* sum <= n  →  non‑abundant (including perfect and deficient). */
    return (seq_t)(sum <= (seq_t)n ? 1 : 0);
}
