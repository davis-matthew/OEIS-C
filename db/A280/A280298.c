/*
 * A280298  Numbers with 67 divisors.
 * https://oeis.org/A280298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000040(n)^66
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 2, big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    uint32_t p = get_prime(n);          /* nth prime (1‑based index) */
    return big_pow((seq_t)p, (seq_t)66);/* p raised to the 66th power */
}
