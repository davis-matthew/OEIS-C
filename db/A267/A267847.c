/*
 * A267847  Decimal representation of the n-th iteration of the "Rule 227"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4, a(n) = 2*(4^n-3) for n>=2 (equivalently,
 *            a(n)=5*a(n-1)-4*a(n-2) for n>3)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0ULL) return (seq_t)1;
    if (n == 1ULL) return (seq_t)4;

    /* fast exponentiation: compute 4^n */
    seq_t result = 1;
    seq_t base   = 4;
    idx_t exp = n;
    while (exp) {
        if (exp & 1ULL) result *= base;
        base *= base;
        exp >>= 1;
    }

    /* a(n) = 2*4^n - 6 */
    result *= 2;
    result -= 6;
    return result;
}
