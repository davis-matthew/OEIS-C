/*
 * A292648  Rank of Pi*n when all the numbers Pi*j and (Pi+1)*k, for j>=1,
 *          k>=1, are jointly ranked.
 * https://oeis.org/A292648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n*Pi/(Pi+1))
 * Range:     u8 n <= 145, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Pi approximated as floor(pi * 10^15) */
    const uint64_t PI_NUM = 3141592653589793ULL; /* floor(pi * 1e15)   */
    const uint64_t PI_DEN = 1000000000000000ULL; /* 1e15                */
    const uint64_t DEN    = PI_NUM + PI_DEN;     /* Pi+1 scaled         */

    unsigned __int128 prod = (unsigned __int128)n * PI_NUM;
    uint64_t t = (uint64_t)(prod / DEN);        /* floor(n*Pi/(Pi+1)) */

    return (seq_t)n + (seq_t)t;
}
