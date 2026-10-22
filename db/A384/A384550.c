/*
 * A384550  Consecutive states of the linear congruential pseudo-random number
 *          generator 2824527309*s mod 2^32 when started at s=1.
 * https://oeis.org/A384550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(n)=2824527309*a(n-1) mod 2^32 */
#include <stdint.h>

/* Compute a(n) for n >= 1.  The sequence is an LCG with multiplier
 * 2824527309 modulo 2^32, starting from state 1. */
seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for offset 0 – return 0 */
        return (seq_t)0;

    idx_t   exp   = n - 1;                     /* exponent n-1 */
    seq_t   res   = (seq_t)1;                  /* result = multiplier^0 */
    seq_t   base  = (seq_t)2824527309u;        /* multiplier */
    const seq_t mask = (seq_t)0xffffffffu;     /* keep low 32 bits */

    while (exp != 0) {
        if (exp & 1ul)
            res = ((res * base) & mask);
        exp >>= 1ul;
        base = ((base * base) & mask);
    }
    return res;
}
