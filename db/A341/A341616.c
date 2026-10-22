/*
 * A341616  Table read by ascending antidiagonals: T(n,j) =
 *          Fibonacci(n)*Lucas(n+j), product of the n-th term in the Fibonacci
 *          sequence (with F(1)=1 and F(2)=1) and the (n+j)-th term in the
 *          Lucas sequence (with L(1)=1 and L(2)=3 and j=0,1,2,...).
 * https://oeis.org/A341616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = Fib(s − (n − (s−1)s/2) + 1) * Lucas(s), where s is the
 *            least positive integer with s*(s+1)/2 ≥ n
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_pair.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    idx_t s = 1;
    while ((s * (s + 1)) / 2 < n) {
        ++s;
    }

    idx_t prev   = (s - 1) * s / 2;
    idx_t offset = n - prev;          /* 1 … s */
    idx_t n_pair = s - offset + 1;    /* 1 … s */

    /* Fibonacci(n_pair) */
    seq_t fn, fn_next;
    fib_pair(n_pair, &fn, &fn_next);

    /* Lucas(s) = 2*Fib(s+1) - Fib(s) */
    seq_t fs, fs_next;
    fib_pair(s, &fs, &fs_next);
    seq_t lucas = fs_next * 2 - fs;

    return fn * lucas;
}
