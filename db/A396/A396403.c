/*
 * A396403  Array read by ascending antidiagonals: A(n,k) = k^(n*(n - 1)/2)
 *          for k >= 0.
 * https://oeis.org/A396403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   array read by ascending antidiagonals: * A(n,k) = k^(n*(n-1)/2),
 *            k >= 0 * a(n) is obtained by scanning all pairs (i,j) with i,j ≥
 *            0 in order * of increasing i+j, and within each antidiagonal
 *            processing i from * the largest to the smallest (i.e. j from 0
 *            upwards)
 * Range:     u8 n <= 29, u16 n <= 46, u32 n <= 57, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Find diagonal s such that cumulative count up to s-1 <= n < cumulative count up to s. */
    uint64_t s = 0;                     /* current diagonal index */
    unsigned __int128 cum = 1;          /* cumulative count up to diagonal s */
    unsigned __int128 prev_cum = 0;     /* cumulative count up to diagonal s-1 */

    while (cum <= n)
    {
        prev_cum = cum;
        s++;
        /* length of diagonal s is s+1 */
        cum += (unsigned __int128)(s + 1);
    }

    /* Position inside the diagonal */
    uint64_t offset = (uint64_t)(n - prev_cum);   /* 0‑based offset */
    uint64_t i = s - offset;                      /* row index (n in the array) */
    uint64_t k = offset;                          /* column index (k in the array) */

    /* Compute exponent i*(i-1)/2 */
    uint64_t exp = (i == 0) ? 0 : i * (i - 1) / 2;

    /* Evaluate k^exp, using the provided utility */
    return pow_uint(k, exp);
}
