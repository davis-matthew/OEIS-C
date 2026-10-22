/*
 * A372198  The number of ways of 2-coloring the diagram for the n-th centered
 *          square number up to rotation and reflection of the square.
 * https://oeis.org/A372198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/8*(2^A001844(n) + 2*2^((n+1)^2) + 2*2^A000124(n) +
 *            2^A002061(n+1) + 2*2^A014848(n+1))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* exponents for the Burnside terms */
    idx_t N = 2*n*(n+1) + 1;                 /* A001844(n)   */
    idx_t A = (n+1)*(n+1);                   /* (n+1)^2      */
    idx_t B = n*(n+1)/2 + 1;                 /* A000124(n)   */
    idx_t C = n*(n+1) + 1;                   /* A002061(n+1) */
    idx_t D = n*(n+1) + (n/2) + 1;           /* A014848(n+1) */

    /* 2 to the required powers */
    seq_t termN = seq_pow((seq_t)2, N);
    seq_t termA = seq_pow((seq_t)2, A);
    seq_t termB = seq_pow((seq_t)2, B);
    seq_t termC = seq_pow((seq_t)2, C);
    seq_t termD = seq_pow((seq_t)2, D);

    /* Burnside sum: 1*termN + 2*termA + 2*termB + 1*termC + 2*termD */
    seq_t sum = termN;
    sum += termA * ((seq_t)2);
    sum += termB * ((seq_t)2);
    sum += termC;
    sum += termD * ((seq_t)2);

    /* divide by 8 to get the number of inequivalent colorings */
    return sum / ((seq_t)8);
}
