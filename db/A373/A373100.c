/*
 * A373100  Last digit of n*2^n - 1.
 * https://oeis.org/A373100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = last digit of n*2^n - 1, i.e. (n*2^n-1) mod 10
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute 2^n mod 10 using fast exponentiation. */
    uint64_t result = 1;
    uint64_t base   = 2;
    idx_t exp = n;
    while (exp) {
        if (exp & 1) result = (result * base) % 10;
        base = (base * base) % 10;
        exp >>= 1;
    }
    uint64_t nmod10 = n % 10;
    uint64_t last = (nmod10 * result) % 10;
    last = (last + 9) % 10;          /* subtract 1, modulo 10 */
    return (seq_t)last;
}
