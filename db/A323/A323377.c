/*
 * A323377  Square array read by ascending antidiagonals: T(n,k) =
 *          Kronecker(prime(n)/prime(k)), n, k >= 1.
 * https://oeis.org/A323377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   T(n,k) = Kronecker(p(n)/p(k)); a(n) is the value of T at the
 *            n‑th position when the infinite square array T is read by
 *            ascending antidiagonals with the entries on each antidiagonal
 *            ordered with i decreasing
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   get_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    /* find diagonal d such that d*(d+1)/2 >= n */
    idx_t d = 1;
    idx_t cum = 1;                 /* cumulative elements up to current diagonal */
    while (cum < n) {
        ++d;
        cum += d;
    }
    idx_t prev   = cum - d;        /* cumulative elements up to previous diagonal */
    idx_t offset = n - prev;       /* 1‑based position within this diagonal */

    idx_t i = d - offset + 1;      /* row index (descending order on the diagonal) */
    idx_t j = offset;              /* column index */

    uint32_t pi = get_prime(i);    /* prime(i) */
    uint32_t pj = get_prime(j);    /* prime(j) */

    int symbol;
    if (pj == 2) {
        /* Kronecker symbol with denominator 2 */
        if ((pi & 1) == 0) {          /* pi even */
            symbol = 0;
        } else {
            uint32_t r = pi % 8;
            symbol = (r == 1 || r == 7) ? 1 : -1;
        }
    } else {
        /* pj is an odd prime; use Jacobi symbol */
        symbol = jacobi_symbol((uint64_t)pi, (uint64_t)pj);
    }
    return (seq_t)symbol;
}
