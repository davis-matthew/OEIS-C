/*
 * A325000  Array read by descending antidiagonals: T(n,k) is the number of
 *          unoriented colorings of the facets (or vertices) of a regular
 *          n-dimensional simplex using up to k colors.
 * https://oeis.org/A325000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(s, r) where * s = ceil((1+sqrt(1+8*n))/2) (the
 *            anti‑diagonal index) * r = n - (s-1)(s-2)/2 + 1 (position inside
 *            that diagonal) * a(n) = T(r, s-r) = C(s, r+1), the array read by
 *            descending * antidiagonals of the triangle T(n,k)=C(n+k,n+1)
 * Range:     u8 n <= 23, u16 n <= 157, u32 n <= 571, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Return the n‑th term of A325000, 1‑based indexing. */
seq_t a(idx_t n)
{
    /* Compute the anti‑diagonal index s such that
     *   (s-1)*s/2 < n <= s*(s-1)/2  */
    idx_t s = 2;
    while ((s * (s - 1)) / 2 < n)
        ++s;

    /* Start index of this anti‑diagonal in the sequence. */
    idx_t start = ((s - 1) * (s - 2)) / 2 + 1;

    /* Offset within the diagonal (0‑based). */
    idx_t offset     = n - start;
    idx_t n_in_group = offset + 1;          /* row index (n) in T(n,k) */
    /* k = s - n_in_group;  // not needed explicitly */

    /* The term is C(s, n_in_group + 1). */
    return binomial(s, n_in_group + 1);
}
