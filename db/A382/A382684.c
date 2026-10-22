/*
 * A382684  Consecutive internal states of the linear congruential
 *          pseudo-random number generator for BCPL when started at 1.
 * https://oeis.org/A382684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2147001325 * a(n-1) + 715136305) mod 2^32 (a(1)=1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute by fast exponentiation of the 2×2 transformation matrix. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1u)
        return (seq_t)1u;               /* a(1) = 1 */

    /* Transformation matrix: [ c  d ; 0 1 ] where
       c = 2147001325, d = 715136305,  modulus = 2^32. */
    static const uint32_t C = 2147001325u;
    static const uint32_t D = 715136305u;

    uint32_t M[2][2] = { {C, D}, {0u, 1u} };   /* current matrix (power of two) */
    uint32_t R[2][2] = { {1u, 0u}, {0u, 1u} };   /* identity matrix (result) */

    idx_t e = n - 1u;       /* exponent = n-1 */

    while (e) {
        if (e & 1u) {        /* R = R * M (mod 2^32) */
            uint32_t T[2][2];
            T[0][0] = (uint64_t)R[0][0] * M[0][0] + (uint64_t)R[0][1] * M[1][0];
            T[0][1] = (uint64_t)R[0][0] * M[0][1] + (uint64_t)R[0][1] * M[1][1];
            T[1][0] = (uint64_t)R[1][0] * M[0][0] + (uint64_t)R[1][1] * M[1][0];
            T[1][1] = (uint64_t)R[1][0] * M[0][1] + (uint64_t)R[1][1] * M[1][1];
            R[0][0] = (uint32_t)(T[0][0] & 0xffffffffu);
            R[0][1] = (uint32_t)(T[0][1] & 0xffffffffu);
            R[1][0] = (uint32_t)(T[1][0] & 0xffffffffu);
            R[1][1] = (uint32_t)(T[1][1] & 0xffffffffu);
        }

        /* M = M * M (mod 2^32) */
        uint32_t T[2][2];
        T[0][0] = (uint64_t)M[0][0] * M[0][0] + (uint64_t)M[0][1] * M[1][0];
        T[0][1] = (uint64_t)M[0][0] * M[0][1] + (uint64_t)M[0][1] * M[1][1];
        T[1][0] = (uint64_t)M[1][0] * M[0][0] + (uint64_t)M[1][1] * M[1][0];
        T[1][1] = (uint64_t)M[1][0] * M[0][1] + (uint64_t)M[1][1] * M[1][1];
        M[0][0] = (uint32_t)(T[0][0] & 0xffffffffu);
        M[0][1] = (uint32_t)(T[0][1] & 0xffffffffu);
        M[1][0] = (uint32_t)(T[1][0] & 0xffffffffu);
        M[1][1] = (uint32_t)(T[1][1] & 0xffffffffu);

        e >>= 1u;
    }

    /* a(n) = first component of R * [a(1);1]  (mod 2^32) */
    uint32_t result = (uint32_t)(((uint64_t)R[0][0] * 1u + (uint64_t)R[0][1]) & 0xffffffffu);

    return (seq_t)result;
}
