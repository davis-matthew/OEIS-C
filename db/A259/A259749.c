/*
 * A259749  Numbers that are congruent to {1,2,5,7,10,11,13,17,19,23} mod 24.
 * https://oeis.org/A259749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*((n-1)/10) + residue[(n-1)%10] where residue =
 *            {1,2,5,7,10,11,13,17,19,23}
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t residues[10] = {
    (seq_t)1,  (seq_t)2,  (seq_t)5,  (seq_t)7,
    (seq_t)10, (seq_t)11, (seq_t)13, (seq_t)17,
    (seq_t)19, (seq_t)23
};

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* offset is 1; behavior below offset is irrelevant */
    /* Compute quotient and remainder in the pattern of 10 numbers per 24. */
    idx_t q = (n - 1) / 10;          /* integer division */
    idx_t r = (n - 1) % 10;          /* 0 .. 9 */
    seq_t rem = residues[(size_t)r];
    /* Combine to form the nth term. */
    return (seq_t)(q * 24 + (idx_t)rem);
}
