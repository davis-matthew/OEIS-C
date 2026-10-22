/*
 * A341553  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(4,n) (with n at least 4) missing three edges, where all
 *          three removed edges are incident to different vertices in the
 *          4-point set but exactly two removed edg...
 * https://oeis.org/A341553
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 21*15^(n-2) - 4*7^(n-1) + 11*3^(n-2) - 1 (n >= 4)
 * Range:     u8 none, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 4) return 0;   /* behaviour for n < 4 is irrelevant */

    seq_t term1 = pow_seq(15, n - 2) * 21;
    seq_t term2 = pow_seq(7, n - 1) * 4;
    seq_t term3 = pow_seq(3, n - 2) * 11;

    seq_t result = term1 - term2 + term3 - 1;
    return result;
}
