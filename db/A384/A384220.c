/*
 * A384220  Consecutive states of the linear congruential pseudo-random number
 *          generator for Smalltalk-80 when started at 1.
 * https://oeis.org/A384220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (27181 * a(n-1) + 13849) mod 2^16, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>


seq_t a(idx_t n)
{
    /* constants for the LCG */
    const uint32_t C = 27181u;      /* multiplier */
    const uint32_t D = 13849u;      /* increment */
    const uint32_t MASK = 0xFFFFu;  /* mod 2^16 */

    /* exponent (n-1) as 64‑bit unsigned */
    uint64_t e = (uint64_t)(n - 1);

    /* current matrix row: [r0 r1] represents [[r0 r1],[0 1]] */
    uint32_t r0 = 1u;   /* identity matrix */
    uint32_t r1 = 0u;

    /* base matrix row: [[C D],[0 1]] */
    uint32_t b0 = C;
    uint32_t b1 = D;

    while (e > 0)
    {
        /* multiply result by base when the current bit is set */
        if (e & 1ULL)
        {
            uint32_t new_r0 = (r0 * b0) & MASK;          /* r0*B00 */
            uint32_t new_r1 = ((r0 * b1) + r1) & MASK;   /* r0*B01 + r1 */
            r0 = new_r0;
            r1 = new_r1;
        }

        /* square the base matrix */
        uint32_t nb0 = (b0 * b0) & MASK;               /* B00*B00 */
        uint32_t nb1 = ((b0 * b1) + b1) & MASK;        /* B00*B01 + B11 (which is 1) */
        b0 = nb0;
        b1 = nb1;

        e >>= 1;
    }

    /* final value: a(n) = r0*a(1) + r1  (with a(1)=1) */
    uint32_t result = (r0 + r1) & MASK;

    return (seq_t)result;
}
