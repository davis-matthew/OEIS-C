/*
 * A326302  a(n) = lcm(n, r(n)) where r(n) = A030101(n) corresponds to the
 *          binary reversal of n.
 * https://oeis.org/A326302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = lcm(n, A030101(n)) – binary reverse of n
 * Range:     u8 n <= 18, u16 n <= 262, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   binary_reverse, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the built‑in gcd from utils/gcd.h and
   computes the binary reversal of n directly, avoiding the decimal
   reversal provided by ../utils/rev.h.  It works for any seq_t type
   that supports the usual arithmetic operations and for any idx_t
   that is an unsigned integer. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/binary_reverse.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* lcm(0,0) is defined as 0 */

    seq_t n_val  = (seq_t)n;
    idx_t rev_idx = binary_reverse(n);
    seq_t rev_val = (seq_t)rev_idx;

    seq_t g = gcd(n_val, rev_val);          /* greatest common divisor */
    return (n_val / g) * rev_val;           /* lcm = (n/g) * rev */
}
