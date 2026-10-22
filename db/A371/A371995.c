/*
 * A371995  Triangle read by rows: T(n, k) = binomial(n - k, k) *
 *          subfactorial(k), for n >= 0 and 0 <= k <= floor(n/2).
 * https://oeis.org/A371995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = binomial(r - k, k) * subfactorial(k) where r is the
 *            smallest non‑negative integer such that sum_{i=0}^{r}
 *            (floor(i/2)+1) > n and k = n - sum_{i=0}^{r-1} (floor(i/2)+1).
 *            Thus a(n) is the nth term of the triangle read row by row
 * Range:     u8 n <= 39, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   binomial, subfactorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/subfactorial.h"

seq_t a(idx_t n)
{
    idx_t r = 0;        /* current row index */
    idx_t count = 0;    /* number of elements before row r */

    /* find row r so that total elements up to r-1 <= n < total up to r */
    while (count + (r / 2 + 1) <= n) {
        count += r / 2 + 1;
        ++r;
    }

    idx_t k = n - count;                 /* column within the found row */
    seq_t bin = binomial(r - k, k);      /* binomial coefficient */
    seq_t fact = subfactorial(k);        /* subfactorial of k */
    return bin * fact;
}
