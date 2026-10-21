/*
 * A250492  (n^6)-th Fibonacci number.
 * https://oeis.org/A250492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   fib_pair_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Fibonacci(n^6)
 */

#include <stdint.h>
#include "../utils/fib_pair_seq.h"

/* Main sequence function.  Computes a(n) = Fibonacci(n^6). */
seq_t a(idx_t n)
{
    seq_t nsq = (seq_t)n * (seq_t)n;        /* n^2   */
    seq_t nby4 = nsq * nsq;                /* n^4   */
    seq_t idx = nby4 * nsq;                 /* n^6   */

    seq_t fn, fn1;
    fib_pair_seq(idx, &fn, &fn1);
    return fn;
}
