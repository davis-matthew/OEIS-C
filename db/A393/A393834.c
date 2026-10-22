/*
 * A393834  A variant of Sierpinski carpet, read by antidiagonals.
 * https://oeis.org/A393834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   not stated
 * Range:     u8 n <= 8723, u16 n <= 8723, u32 n <= 8723, u64 n <= 8723,
 *            u128 n <= 8723, big any n
 * Helpers:   cell
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) is obtained by reading the array A(i,j) along antidiagonals.
 *   For i,j ≥ 0, A(i,j) = 1, except A(i,j) = 0 whenever the number
 *   of positions in the base‑3 representations of i and j where both
 *   digits equal 1 is odd.
 *   In other words, let c be the parity of
 *          ∑_{t≥0} [ ( (i / 3ᵗ) % 3 == 1 ) && ( (j / 3ᵗ) % 3 == 1 ) ].
 *   Then A(i,j) = 0 if c is odd, otherwise 1.
 *
 * The array is symmetric, satisfies A(3n,3k)=A(n,k) and A(n,2n)=1,
 * and this parity rule matches the OEIS definition of A393834.
 */

#include <stdbool.h>
#include "../utils/cell.h"

/* Convert linear index n into coordinates (i,j) on the antidiagonals. */
static void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    idx_t d = 0;          /* current diagonal index */
    idx_t count = 0;      /* number of elements processed so far */

    /* Find smallest d such that n < (d+1)(d+2)/2   */
    while (count + d + 1 <= n) {
        count += d + 1;
        ++d;
    }

    *i = n - count;          /* offset within the diagonal */
    *j = d - *i;              /* complement to d */
}

/* Public sequence function */
seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);
    return cell(i, j);
}
