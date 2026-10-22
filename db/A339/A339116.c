/*
 * A339116  Triangle of all squarefree semiprimes grouped by greater prime
 *          factor, read by rows.
 * https://oeis.org/A339116
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1276, u32 n <= 1276, u64 n <= 1276,
 *            u128 n <= 1276, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: A339116 – Triangle of all squarefree semiprimes grouped
   by greater prime factor, read by rows.
   a(n) = prime(i) * prime(j)  where a(n) is the nth term (n ≥ 2),
   i > j, and the pair (i,j) is the position of the term in the
   lower‑triangular table read row by row (row 2 starts with i = 2). */

#include <stdint.h>
#include "../utils/get_prime.h"

/* Return the nth term of A339116 (offset 2). */
seq_t a(idx_t n)
{
    if (n < 2)               /* undefined for indices below the offset */
        return 0;

    /* Zero–based offset inside the triangle. */
    idx_t m = n - 2;

    /* Find row index i such that (i-1)*i/2 > m  (minimal i).  */
    idx_t i = 1;
    while ((i - 1) * i / 2 <= m)
        i++;

    /* Number of elements preceding this row. */
    idx_t prev_total = (i - 2) * (i - 1) / 2;

    /* Column index j within the row (1‑based). */
    idx_t j = m - prev_total + 1;

    /* The value is the product of the j‑th and i‑th primes. */
    seq_t p_i = (seq_t)get_prime(i);
    seq_t p_j = (seq_t)get_prime(j);
    return p_i * p_j;
}
