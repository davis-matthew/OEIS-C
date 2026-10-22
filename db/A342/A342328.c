/*
 * A342328  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(6,n) (with n at least 4) missing three edges, where all
 *          three removed edges are incident to different vertices in the
 *          6-point set but exactly two removed edg...
 * https://oeis.org/A342328
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 465*63^(n-2) - 982*31^(n-2) + 807*15^(n-2) - 316*7^(n-2)
 *            + 56*3^(n-2) - 3 offset: 4 (so a(4) is the first term)
 * Range:     u8 none, u16 none, u32 n <= 5, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>          /* for bool, if needed */
#include "../utils/big_pow.h" /* big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    if (n < 4)          /* values below offset are irrelevant */
        return (seq_t)0;

    /* exponent for all terms */
    seq_t exp = (seq_t)(n - 2);

    /* pre‑compute the powers */
    seq_t p63 = big_pow((seq_t)63, exp);
    seq_t p31 = big_pow((seq_t)31, exp);
    seq_t p15 = big_pow((seq_t)15, exp);
    seq_t p7  = big_pow((seq_t)7 , exp);
    seq_t p3  = big_pow((seq_t)3 , exp);

    /* compute the positive part */
    seq_t pos = (seq_t)465 * p63;
    pos += (seq_t)807 * p15;
    pos += (seq_t)56  * p3;

    /* compute the negative part */
    seq_t neg = (seq_t)982 * p31;
    neg += (seq_t)316 * p7;
    neg += (seq_t)3;   /* the constant term */

    /* final result */
    seq_t res = pos;
    res -= neg;              /* pos > neg for all n >= 4 */
    return res;
}
