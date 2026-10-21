/*
 * A275748  Number of Fibonacci numbers in {n, f(n), f(f(n)), ...., 1}, where
 *          f is the Collatz function.
 * https://oeis.org/A275748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) = number of Fibonacci numbers in the Collatz trajectory
 * of n, including the starting value n and the terminating 1. */
#include <stdbool.h>
#include "../utils/floor_sqrt.h"  /* provides seq_t floor_sqrt(seq_t) */

static bool is_fib(seq_t x)
{
    /* Check whether x is a Fibonacci number using the
       property: 5*x^2+4 or 5*x^2-4 is a perfect square. */
    seq_t y1 = 5 * x * x + 4;
    seq_t y2 = 5 * x * x - 4;
    seq_t r1  = floor_sqrt(y1);
    if (r1 * r1 == y1) return true;
    seq_t r2  = floor_sqrt(y2);
    return r2 * r2 == y2;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t cur   = (seq_t)n;          /* start of the trajectory */

    for (;;) {
        if (is_fib(cur))
            count++;

        if (cur == (seq_t)1)         /* reached the final state */
            break;

        if (cur % (seq_t)2 == (seq_t)0)
            cur = cur / (seq_t)2;    /* even step */
        else
            cur = (seq_t)3 * cur + (seq_t)1;  /* odd step */
    }

    return count;
}
