/*
 * A320531  T(n,k) = n*k^(n - 1), k > 0, with T(n,0) = A063524(n), square
 *          array read by antidiagonals upwards.
 * https://oeis.org/A320531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) reads the square array T(n,k) = n*k^(n-1) (k>0), * with
 *            T(n,0) = 1 if n==1 else 0, by antidiagonals upwards
 * Range:     u8 n <= 38, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

static seq_t T_nk(idx_t row, idx_t col)
{
    if (col == 0) {
        /* T(n,0) is 1 only for n = 1, 0 otherwise. */
        return (seq_t)(row == 1 ? 1 : 0);
    }
    if (row == 0) {
        /* For n = 0 and k > 0, the definition gives 0. */
        return (seq_t)0;
    }
    /* k > 0 and n > 0: T(n,k) = n * k^(n-1). */
    seq_t pow = pow_uint((uint64_t)col, row - 1);
    return (seq_t)row * pow;
}

seq_t a(idx_t n)
{
    /* Find the diagonal s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2. */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n) {
        ++s;
    }

    /* Position inside the diagonal (d goes from 0 up to s). */
    idx_t d = n - s * (s + 1) / 2;
    idx_t row = s - d;
    idx_t col = d;

    return T_nk(row, col);
}
