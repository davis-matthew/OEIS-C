/*
 * A247310  Rectangular array read upwards by columns: T = T(n,k) = number of
 *          paths from (0,1) to (n,k), where 0 <= k <= 2, consisting of
 *          segments given by the vectors (1,1), (1,2), (1,-1), (1,-2), where
 *          no segment is followed by a segment in the same direction.
 * https://oeis.org/A247310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = T(c,r), where c = n / 3 and r = n % 3 (r=0 bottom, r=1
 *            middle, r=2 top). • middle row (r = 1): a(n) = Fib(c) • bottom
 *            row (r = 0) & c even: a(n) = Fib(c + 1) • top row (r = 2) & c
 *            odd : a(n) = Fib(c + 1) • otherwise: a(n) = 0 Fib(k) is the k‑th
 *            Fibonacci number with Fib(0)=0, Fib(1)=1
 * Range:     u8 n <= 40, u16 n <= 71, u32 n <= 142, u64 n <= 280,
 *            u128 n <= 557, big any n
 * Helpers:   fib_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib_idx.h"

seq_t a(idx_t n)
{
    idx_t c = n / 3;
    idx_t r = n % 3;

    if (r == 1) {                     /* middle row  */
        return fib_idx(c);
    } else if (r == 0) {               /* bottom row  */
        return (c & 1) ? 0 : fib_idx(c + 1);
    } else {                           /* top row     */
        return (c & 1) ? fib_idx(c + 1) : 0;
    }
}
