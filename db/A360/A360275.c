/*
 * A360275  Number of unordered quadruples of self-avoiding paths with nodes
 *          that cover all vertices of a convex n-gon.
 * https://oeis.org/A360275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/3)*n*(n-1)*(n-2)*(n-3)*2^(n-15)*(4^(n-4) - 4*3^(n-4) +
 *            6*2^(n-4) - 4), for n >= 8; otherwise a(n) = 0
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 13, u64 n <= 23, u128 n <= 43,
 *            big any n
 * Helpers:   pow2, pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* Return 0 for n less than 8; the formula also gives 0 for 5,6,7
       because the parenthetical factor becomes zero.          */
    if (n < 8)
        return 0;

    /* Compute the product n*(n-1)*(n-2)*(n-3) as seq_t. */
    seq_t prod = (seq_t)n * (n-1) * (n-2) * (n-3);

    /* Compute the powers needed for the parenthetical factor. */
    seq_t p4 = pow4(n-4);          /* 4^(n-4) */
    seq_t p3 = pow3(n-4);          /* 3^(n-4) */
    seq_t p2 = pow2(n-4);          /* 2^(n-4) */

    /* parenthetical factor: 4^(n-4) - 4*3^(n-4) + 6*2^(n-4) - 4 */
    seq_t paren = p4 - 4*p3 + 6*p2 - 4;

    /* Combine product with parenthetical factor first to keep numbers manageable. */
    seq_t val = prod * paren;

    /* Multiply by 2^(n-15) if n>=15; otherwise divide by 2^(15-n). */
    if (n >= 15)
        val *= pow2(n-15);
    else
        val /= pow2(15-n);

    /* Final division by 3.  The result is always an integer for n>=8. */
    val /= 3;

    return val;
}
