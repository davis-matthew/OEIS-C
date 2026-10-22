/*
 * A384778  Consecutive states of the linear congruential pseudo-random number
 *          generator 55151000561141*s mod 2^48 when started at 1.
 * https://oeis.org/A384778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 55151000561141^(n-1) mod 2^48, a(1)=1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   modmul48
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/modmul48.h"

/* Linear congruential generator recurrence:
   a(1) = 1
   a(n) = 55151000561141 * a(n-1) mod 2^48  for n>1
   Equivalent to a(n) = 55151000561141^(n-1) mod 2^48. */
seq_t a(idx_t n)
{
    if (n == 0)     /* undefined, but return 0 for safety */
        return (seq_t)0;
    if (n == 1)     /* first term explicitly given */
        return (seq_t)1;

    /* Base multiplier of the LCG, already less than 2^48 */
    const seq_t base0 = (seq_t)55151000561141ULL;

    seq_t result = (seq_t)1;          /* value of m^(0) */
    seq_t base   = base0;             /* value of m^(1) */
    idx_t e = n - 1;                  /* exponent */

    while (e) {
        if (e & 1)
            result = modmul48(result, base);
        e >>= 1;
        if (e)
            base = modmul48(base, base);
    }
    return result;
}
