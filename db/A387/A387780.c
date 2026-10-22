/*
 * A387780  Lower (3/2)-midsequence of (F(2n)) and (F(2n+1)), where F=A000045
 *          (Fibonacci numbers); see Comments.
 * https://oeis.org/A387780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((3/2) * F(2*n+2))
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute Fibonacci with utils/fib.h and apply the floor operation */
/* using a bit‑wise trick that never overflows the intermediate result. */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute the index of the required Fibonacci number. */
    idx_t m = n + n + 2;            /* m = 2*n + 2 */

    /* Fibonacci number F(m) in seq_t. */
    seq_t fn = fib(m);

    /* floor(3*fn/2) without overflow:
       if fn is even → 3*(fn/2)
       if fn is odd  → 3*((fn-1)/2) + 1 (which equals ((fn>>1)*3) + 1)   */
    seq_t result;
    if (fn & (seq_t)1) {
        result = (fn >> 1) * (seq_t)3 + (seq_t)1;
    } else {
        result = (fn >> 1) * (seq_t)3;
    }
    return result;
}
