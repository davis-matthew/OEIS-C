/*
 * A400507  Square array A(n,k) = k*(k+2)^(n-1), n >= 1, k >= 1, read by
 *          antidiagonals.
 * https://oeis.org/A400507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl, new
 * Formula:   a(n) = k*(k+2)^(n-1), where (k,n) are the indices in the array
 *            A(n,k) read by antidiagonals
 * Range:     u8 n <= 18, u16 n <= 50, u32 n <= 130, u64 n <= 368,
 *            u128 n <= 1022, big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n < 1 (offset is 1) */

    /* Find diagonal d such that (d-1)*d/2 ≥ n */
    idx_t d = 2;
    while (((d - 1) * d) / 2 < n)
        ++d;

    /* Index of the last element on the previous diagonal */
    idx_t prev = ((d - 2) * (d - 1)) / 2;

    /* Position within the current diagonal, 0‑based */
    idx_t pos = n - prev - 1;

    /* Indices in the array: row = n_, col = k */
    idx_t nrow = pos + 1;               /* n_ */
    idx_t kcol = (d - 1) - pos;         /* k */

    /* Compute k * (k+2)^(nrow-1) */
    seq_t base  = (seq_t)(kcol + 2);
    seq_t power = pow_int(base, (idx_t)(nrow - 1));
    return (seq_t)(kcol) * power;
}
