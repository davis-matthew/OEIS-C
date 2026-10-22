/*
 * A317502  Triangle read by rows: T(0,0) = 1; T(n,k) = 3 T(n-1,k) - 2 *
 *          T(n-3,k-1) for k = 0..floor(n/3); T(n,k)=0 for n or k < 0.
 * https://oeis.org/A317502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, sign, easy
 * Formula:   a(n) is obtained by flattening the triangle of T(n,k) where
 *            T(n,k) = 3^(n-3k) * (-2)^k * binom(n-2k, k). Here binom is
 *            provided by the utils library
 * Range:     i8 n <= 6, i16 n <= 21, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

/* Compute base^exp for integer exp using repeated squaring */
static seq_t seq_pow_int(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Locate the row r and column k corresponding to index n */
    idx_t r = 0;
    idx_t cumulative = 0;
    while (true) {
        idx_t row_len = r / 3 + 1;        /* floor(r/3)+1  */
        if (cumulative + row_len > n)    /* next row would exceed n */
            break;
        cumulative += row_len;
        r++;
    }
    idx_t k = n - cumulative;            /* column within row r */

    /* Compute T(r,k) using the explicit formula */
    idx_t n_minus_2k = r - 2 * k;         /* >= 0 */
    idx_t n_minus_3k = r - 3 * k;         /* >= 0 */

    seq_t val = binom(n_minus_2k, k);    /* binom(r-2k, k) */
    val *= seq_pow_int((seq_t)3,     n_minus_3k);   /* 3^(r-3k)   */
    val *= seq_pow_int((seq_t)(-2), k);           /* (-2)^k      */

    return val;
}
