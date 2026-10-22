/*
 * A340173  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(4,n) (with n at least 3) missing two edges, where the two
 *          removed edges are not incident to the same vertex in the 4-point
 *          set but are incident to the same v...
 * https://oeis.org/A340173
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*15^(n-1) - 8*7^(n-1) + 7*3^(n-1) - 2
 * Range:     u8 none, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Fast exponentiation for the generic type seq_t.
   Works for all supported integer types including arbitrary‑precision. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & (idx_t)1)
            result *= base;
        exp >>= (idx_t)1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* n >= 3 for proper values; behaviour for smaller n is undefined */
    idx_t e = n - 1;

    seq_t pow15 = pow_seq((seq_t)15, e);
    seq_t pow7  = pow_seq((seq_t)7,  e);
    seq_t pow3  = pow_seq((seq_t)3,  e);

    seq_t result = (seq_t)3 * pow15;   /* 3*15^(n-1) */
    result -= (seq_t)8 * pow7;         /* -8*7^(n-1) */
    result += (seq_t)7 * pow3;         /* +7*3^(n-1) */
    result -= (seq_t)2;                /* -2 */

    return result;
}
