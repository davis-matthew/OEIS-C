/*
 * A343373  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(4,n) (with n at least 4) missing three edges, where
 *          exactly two removed edges are incident to the same vertex in the
 *          4-point set and exactly two removed edge...
 * https://oeis.org/A343373
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 21*15^(n-2) - 36*7^(n-2) + 17*3^(n-2) - 2
 * Range:     u8 none, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(4) is the first term of the sequence; values for n < 4 are undefined */

#include <stdbool.h>

/* fast exponentiation for seq_t (uses built‑in multiplication) */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t power  = base;
    while (exp > 0) {
        if (exp & 1U)
            result *= power;
        power *= power;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;                 /* below the offset – arbitrary */

    idx_t e = n - 2;                     /* exponent for the powers */

    seq_t p15 = pow_seq((seq_t)15, e);
    seq_t p7  = pow_seq((seq_t)7,  e);
    seq_t p3  = pow_seq((seq_t)3,  e);

    seq_t result = (seq_t)21 * p15
                 - (seq_t)36 * p7
                 + (seq_t)17 * p3
                 - (seq_t)2;
    return result;
}
