/*
 * A384448  Consecutive states of the linear congruential pseudo-random number
 *          generator for the INMOS Transputer when started at 1.
 * https://oeis.org/A384448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1664525 * a(n-1) mod 2^32, a(1)=1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)                              /* a(1) = 1; ignore n==0 */
        return (seq_t)1;

    idx_t e = n - 1;                          /* exponent = n-1 */
    uint64_t result = 1;                      /* current value (mod 2^32) */
    uint64_t base   = 1664525ULL;             /* multiplier in the recurrence */
    const uint64_t mask = 0xFFFFFFFFULL;      /* keep only the low 32 bits */

    while (e > 0) {
        if (e & 1)
            result = (result * base) & mask;  /* multiply and reduce modulo 2^32 */
        base = (base * base) & mask;          /* square base modulo 2^32 */
        e >>= 1;
    }

    return (seq_t)result;                     /* cast to the build’s seq_t type */
}
