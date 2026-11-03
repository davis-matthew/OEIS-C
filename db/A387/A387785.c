/*
 * A387785  Upper (2/3)-midsequence of (F(2n)) and (F(2n+1)), where F=A000045
 *          (Fibonacci numbers); see Comments.
 * https://oeis.org/A387785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((2/3)*F(2n+2)), where F is Fibonacci
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 29 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

seq_t a(idx_t n)
{
    /* compute m = 2*n + 2 */
    idx_t m = n + n + 2;
    seq_t f = fib(m);          /* F(2n+2) */

    /* ceil(2*f/3) = (f/3)*2 + (f%3)  , no overflow in 128‑bit case  */
    seq_t q = f / (seq_t)3;
    seq_t r = f % (seq_t)3;
    seq_t ans = q * (seq_t)2 + r;
    return ans;
}
