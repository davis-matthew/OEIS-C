/*
 * A353655  Number of terms in the Fibonacci-Lucas representation of n.
 * https://oeis.org/A353655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9881 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A353655: number of terms in the Fibonacci‑Lucas representation of n */
#include <stdbool.h>

/* Return the largest Fibonacci number ≤ v.  v ≥ 1. */
static idx_t largest_fib(idx_t v)
{
    idx_t a = 1, b = 1;          /* F(1), F(2) */
    while (b <= v) {
        idx_t tmp = b;
        b = a + b;
        a = tmp;
    }
    return a;                    /* a ≤ v < b */
}

/* Return the largest Lucas number ≤ v.  v ≥ 1. */
static idx_t largest_lucas(idx_t v)
{
    idx_t a = 2, b = 1;          /* L(0), L(1) */
    while (b <= v) {
        idx_t tmp = b;
        b = a + b;
        a = tmp;
    }
    return a;                    /* a ≤ v < b */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* defined only for n ≥ 1, but guard against 0 */
    idx_t rem = n;
    seq_t count = 0;
    bool fib_turn = true;        /* start with a Fibonacci term */

    while (rem > 0) {
        idx_t t;
        if (fib_turn) {
            t = largest_fib(rem);
        } else {
            t = largest_lucas(rem);
        }
        rem -= t;
        ++count;
        fib_turn = !fib_turn;
    }

    return count;
}
