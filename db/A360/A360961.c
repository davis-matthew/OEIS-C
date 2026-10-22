/*
 * A360961  Triangle T(m,n) read by rows: the number of homomorphisms of the
 *          complete graph on n vertices to the quasi-complete graph on m
 *          vertices, m>=3, 3<=n<m.
 * https://oeis.org/A360961
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(m,n) = n! * ( binomial(m-2,n-1) * 2 + binomial(m-2,n) ), where
 *            m ≥ 4, 3 ≤ n < m. The sequence a(n) enumerates these values by
 *            rows starting at m = 4, n = 3 (offset 3)
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Off‑set of the enumeration (a(3) is the first term) */
    if (n < 3)
        return 0;

    idx_t offset = n - 3;          /* zero‑based index within triangle */

    /* Find the row m such that cumulative count up to m
       exceeds offset: (m-3)(m-2)/2 > offset */
    idx_t low = 4, high = 4;
    while ((high - 3) * (high - 2) / 2 <= offset)
        high++;                     /* lift high until it exceeds offset */
    while (low < high) {
        idx_t mid = (low + high) / 2;
        if ((mid - 3) * (mid - 2) / 2 > offset)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;

    /* Number of entries before this row */
    idx_t prev = (m - 4) * (m - 3) / 2;
    /* Position within the row */
    idx_t pos = offset - prev;
    idx_t k  = 3 + pos;            /* the n in T(m,n) */

    /* Compute T(m,k) using the given formula */
    seq_t fact_k   = factorial(k);
    seq_t b1 = binomial(m - 2, k - 1); /* C(m-2,k-1) */
    seq_t b2 = binomial(m - 2, k);     /* C(m-2,k)   */
    seq_t sum = b1 * 2 + b2;           /* 2*C(m-2,k-1) + C(m-2,k) */
    seq_t result = fact_k * sum;       /* k! * sum */

    return result;
}
