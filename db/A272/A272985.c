/*
 * A272985  Numbers n such that the elements of the Collatz trajectory of n
 *          are exactly the same as the divisors of n.
 * https://oeis.org/A272985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence consists of powers of two, with a single exception:
 *            a(1) = 2^0 = 1 a(2) = 2^1 = 2 … a(7) = 2^6 = 64 a(8) = 80 (the
 *            unique non‑power‑of‑two member) a(9) = 2^7 = 128 a(10)= 2^8 =
 *            256 … In general: if n <= 7 a(n) = 2^(n-1) if n == 8 a(n) = 80
 *            if n >= 9 a(n) = 2^(n-2)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 8)
        return (seq_t)80;

    if (n <= 7)
        return pow2(n - 1);

    /* n >= 9 */
    return pow2(n - 2);
}
