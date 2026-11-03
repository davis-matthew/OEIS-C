/*
 * A377208  a(n) is the number of iterations that n requires to reach a
 *          noninteger or a Fibonacci number under the map x -> x / z(x),
 *          where z(k) = A007895(k) is the number of terms in the Zeckendorf
 *          representation of k; a(n) = 0 if n is a Fibonacci number.
 * https://oeis.org/A377208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number of iterations of x -> x / z(x) * where z(k) =
 *            number of terms in the Zeckendorf * representation of k. The
 *            process stops when a * non‑integer or a Fibonacci number is
 *            reached. * a(n) = 0 if n itself is a Fibonacci number
 * Range:     u8 n <= 2839, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9598 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Test whether a positive integer n is a Fibonacci number
 * in the Zeckendorf Fibonacci sequence 1,2,3,5,8,... */
static bool is_fibonacci(seq_t n)
{
    if (n == (seq_t)1 || n == (seq_t)2)
        return true;

    seq_t a = (seq_t)1, b = (seq_t)2;
    while (b < n) {
        seq_t c = a + b;
        a = b;
        b = c;
    }
    return n == b;
}

/* Return the number of terms in the Zeckendorf
 * representation of a positive integer x. */
static seq_t zeckendorf_terms(seq_t x)
{
    if (x < (seq_t)1) return 0;

    /* Build Fibonacci numbers 1,2,3,5,... up to x */
    seq_t fibs[100];
    int fc = 0;
    fibs[fc++] = (seq_t)1;
    fibs[fc++] = (seq_t)2;
    while (fibs[fc-1] < x) {
        fibs[fc] = fibs[fc-1] + fibs[fc-2];
        if (fibs[fc] < fibs[fc-1])        /* overflow guard (unlikely for small n) */
            break;
        fc++;
    }

    /* Greedy decomposition */
    seq_t r = x;
    int terms = 0;
    int i = fc - 1;
    while (r > (seq_t)0) {
        while (r < fibs[i])
            i--;
        r -= fibs[i];
        terms++;
    }
    return (seq_t)terms;
}

/* Main sequence function as required. */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;

    if (is_fibonacci(x))
        return (seq_t)0;

    seq_t count = (seq_t)0;
    while (true) {
        seq_t z = zeckendorf_terms(x);
        seq_t div = x / z;
        count++;

        /* Stop if the result is not an integer. */
        if (x % z != (seq_t)0)
            return count;

        /* Stop if the new integer is a Fibonacci number. */
        if (is_fibonacci(div))
            return count;

        x = div;
    }
}
