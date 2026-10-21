/*
 * A264745  Rectangular array A read by upward antidiagonals in which the
 *          entry in row n and column k is defined by A(n,k) =
 *          Fibonacci(2^(n-1)*(2*k-1) + 1), n,k >= 1.
 * https://oeis.org/A264745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Fibonacci( 2^(row-1) * (2*col-1) + 1 ), where (row, col)
 *            are the antidiagonal coordinates of n (row, col ≥ 1). The
 *            sequence A264745 is read by upward antidiagonals of the
 *            rectangular array A(n,k)
 * Range:     u8 n <= 10, u16 n <= 11, u32 n <= 16, u64 n <= 22, u128 n <= 29,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* includes */
#include <stdint.h>
#include "../utils/bitlen_seq_t.h"

/* fast doubling Fibonacci for an index that fits in seq_t */
static seq_t fib_big(seq_t n)
{
    if (n == 0) return 0;
    seq_t a = 0;      /* F(k)   */
    seq_t b = 1;      /* F(k+1) */
    unsigned int bits = bitlen_seq_t(n);
    unsigned int i;
    for (i = bits - 1; ; --i) {
        seq_t two_b = b + b;                        /* 2*b   */
        seq_t c = a * (two_b - a);                 /* F(2k) */
        seq_t d = a * a + b * b;                   /* F(2k+1) */
        if ((n >> i) & 1) {                        /* next bit is 1 */
            a = d;
            b = c + d;
        } else {                                    /* next bit is 0 */
            a = c;
            b = d;
        }
        if (i == 0) break;
    }
    return a;   /* F(n) */
}

/* Main entry point for the OEIS sequence */
seq_t a(idx_t n)
{
    /* Find the antidiagonal t such that t*(t+1)/2 ≥ n */
    idx_t t = 1;
    while ((t * (t + 1)) / 2 < n)
        ++t;

    /* Number of elements before this diagonal */
    idx_t prev = (t - 1) * t / 2;

    /* Position inside the diagonal (1‑based) */
    idx_t pos = n - prev;

    /* Coordinates: row i and column j (both 1‑based) */
    idx_t i = t - pos + 1;
    idx_t j = pos;

    /* Compute 2^(i-1) as seq_t */
    seq_t pow2 = 1;
    for (idx_t k = 1; k <= i - 1; ++k)
        pow2 *= 2;

    /* Compute (2*j-1) as seq_t */
    seq_t odd = (seq_t)(2 * j - 1);

    /* Argument to Fib: 2^(i-1)*(2*j-1)+1 */
    seq_t idx = pow2 * odd + 1;

    return fib_big(idx);
}
