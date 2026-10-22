/*
 * A371241  Euler totient function applied to the zeroless numbers in base 3
 *          (A032924).
 * https://oeis.org/A371241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = phi( zeroless_base3(n) ) where zeroless_base3(n) is the
 *            n‑th integer with base‑3 representation consisting only of
 *            digits 1 and 2 (OEIS A032924)
 * Range:     u8 n <= 62, u16 n <= 2046, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Find the number of ternary digits d for the n‑th zeroless base‑3
       number: 2^(d+1)‑2 >= n.  The loop chooses the smallest d
       satisfying that inequality. */
    idx_t d = 0;
    while (((idx_t)1 << (d + 1)) < n + 2) {
        ++d;
    }

    /* Zero‑based index inside the block of d‑digit numbers. */
    idx_t prev   = ((idx_t)1 << d) - 2;   /* count of numbers with < d digits */
    idx_t offset = n - prev - 1;          /* 0 <= offset < 2^d */

    /* Build the zeroless base‑3 number from its binary description.
       Each binary bit corresponds to a ternary digit: 0 → 1, 1 → 2. */
    seq_t val = 0;
    for (idx_t i = 0; i < d; ++i) {
        idx_t bit  = (offset >> (d - 1 - i)) & 1;
        val = val * (seq_t)3 + (bit ? (seq_t)2 : (seq_t)1);
    }

    /* Return its Euler totient. */
    return phi((idx_t)val);
}
