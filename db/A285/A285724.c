/*
 * A285724  Square array read by descending antidiagonals: If n > k, A(n,k) =
 *          T(lcm(n,k), gcd(n,k)), otherwise A(n,k) = T(gcd(n,k), lcm(n,k)),
 *          where T(n,k) is sequence A000027 considered as a two-dimensional
 *          table.
 * https://oeis.org/A285724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value obtained by reading square array A(n,k) on
 *            descending antidiagonals. A(n,k) = T(lcm(n,k),gcd(n,k)) if n>k,
 *            otherwise A(n,k) = T(gcd(n,k),lcm(n,k)), where T is Cantor's
 *            pairing function T(x,y) = (x + y - 1)*(x + y - 2)/2 + x
 * Range:     u8 n <= 31, u16 n <= 718, u32 n <= 9899, u64 n <= 9899,
 *            u128 n <= 9899, big any n
 * Helpers:   cantor_pair, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/cantor_pair.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* n>=1 for the sequence */
    idx_t S = 2;
    while ((S * (S - 1)) / 2 < n) ++S;   /* find diagonal number S */
    idx_t preceding = (S - 2) * (S - 1) / 2;
    idx_t i = n - preceding;    /* row index */
    idx_t j = S - i;            /* column index */

    seq_t ai = (seq_t)i;
    seq_t aj = (seq_t)j;
    seq_t g = gcd(ai, aj);     /* gcd(ai,aj) */
    seq_t l = (ai / g) * aj;    /* lcm(ai,aj) */

    if (ai > aj)
        return cantor_pair(l, g);
    else
        return cantor_pair(g, l);
}
