/*
 * A355044  Number of coalescent histories for matching gene trees and species
 *          trees with n leaves and a 5-leaf seed tree.
 * https://oeis.org/A355044
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((23*n^2-131*n+180)/(4*(2*n-3)*(2*n-5))) * Catalan(n-1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   catalan, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* OEIS offset: 5 – values below are undefined, return 0. */
    if (n < 5)
        return (seq_t)0;

    /* ----- compute the rational factor ----------------------------------- */
    seq_t N = (seq_t)23 * n * n
            - (seq_t)131 * n
            + (seq_t)180;

    seq_t D = (seq_t)4 *
              ((seq_t)2 * n - (seq_t)3) *
              ((seq_t)2 * n - (seq_t)5);

    /* Reduce the fraction N/D as much as possible. */
    seq_t g1 = gcd(N, D);
    N /= g1;
    D /= g1;

    /* ----- compute the Catalan number ------------------------------------ */
    idx_t k = n - (idx_t)1;          /* k = n-1, the argument for Catalan   */
    seq_t cat = catalan(k);

    /* Reduce cat with the remaining denominator. */
    seq_t g2 = gcd(cat, D);
    cat /= g2;
    D /= g2;

    /* Any remaining division must divide the product exactly. */
    if (D != (seq_t)1)
        cat = cat / D;          /* exact division, D is guaranteed to divide */

    /* ----- final result -------------------------------- */
    return cat * N;
}
