/*
 * A335613  Number of sets (in the Hausdorff metric geometry) at each location
 *          between two sets defined by a complete bipartite graph K(4,n)
 *          (with n at least 3) missing two edges, where the removed edges are
 *          incident to the same vertex in the four point part.
 * https://oeis.org/A335613
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 49*15^(n-2) - 76*7^(n-2) + 10*3^(n-1) - 3
 * Range:     u8 none, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 3  (a(3) is the first term) */

#include "../utils/seq_mul.h"   /* seq_mul(seq_t,seq_t) */
#include "../utils/seq_pow.h"   /* seq_pow(seq_t,idx_t) */

seq_t a(idx_t n)
{
    /* Sequence is only defined for n ≥ 3; below that we return 0. */
    if (n < 3)
        return 0;

    /* Exponents for the powers. */
    idx_t exp15_7 = n - 2;   /* for 15^(n-2) and 7^(n-2) */
    idx_t exp3    = n - 1;   /* for 3^(n-1) */

    /* Compute the required powers using the provided seq_pow helper. */
    seq_t pow15 = seq_pow((seq_t)15, exp15_7);
    seq_t pow7  = seq_pow((seq_t)7,  exp15_7);
    seq_t pow3  = seq_pow((seq_t)3,  exp3);

    /* Compute each term of the formula using seq_mul for exact arithmetic. */
    seq_t term1 = seq_mul(pow15, (seq_t)49);  /* 49*15^(n-2)   */
    seq_t term2 = seq_mul(pow7,  (seq_t)76);  /* 76*7^(n-2)    */
    seq_t term3 = seq_mul(pow3,  (seq_t)10);  /* 10*3^(n-1)    */

    /* Combine them: 49*15^(n-2) - 76*7^(n-2) + 10*3^(n-1) - 3 */
    seq_t result = term1 - term2 + term3;
    result -= (seq_t)3;

    return result;
}
