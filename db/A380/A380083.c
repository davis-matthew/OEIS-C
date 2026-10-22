/*
 * A380083  a(n) = Pell(n^2)/Pell(n).
 * https://oeis.org/A380083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Pell(n^2) / Pell(n) (offset 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   pell_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pell_pair.h"

/* Sequence function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    /* For the requested offset (1) no values are defined for n==0. */
    if (n == 0) return 0;

    /* Compute Pell(n) */
    seq_t p_n, p_n1;
    pell_pair(n, &p_n, &p_n1);

    /* Compute Pell(n^2) */
    idx_t n2 = n * n;          /* n <= 10000 so the product fits in idx_t */
    seq_t p_n2, p_n2_1;
    pell_pair(n2, &p_n2, &p_n2_1);

    /* a(n) = Pell(n^2) / Pell(n)  ; division is exact         */
    return p_n2 / p_n;
}
