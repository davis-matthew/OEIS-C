/*
 * A300895  L.g.f.: log(Product_{k>=2} (1 + x^Fibonacci(k))) = Sum_{n>=1}
 *          a(n)*x^n/n.
 * https://oeis.org/A300895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{d|n, d is Fibonacci} (-1)^{(n/d)+1} * d
 * Range:     i8 n <= 85, i16 n <= 85, i32 n <= 85, i64 n <= 85, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static table of Fibonacci numbers up to the largest n seen. */
    static idx_t *fibs = NULL;
    static size_t fib_cnt = 0, fib_cap = 0;
    static idx_t last_fib = 0;

    /* Initialise Fibonacci table if this is the first call. */
    if (!fibs) {
        fib_cap = 32;                                   /* minimal capacity */
        fibs = (idx_t*)malloc(sizeof(idx_t) * fib_cap);
        if (!fibs) return 0; /* unlikely, but safe */
        fibs[0] = 1;          /* F_2 */
        fibs[1] = 2;          /* F_3 */
        fib_cnt = 2;
        last_fib = 2;
    }

    /* Extend the table until it contains all Fibonacci numbers ≤ n. */
    while (last_fib <= n) {
        /* Compute the next Fibonacci number with the recurrence. */
        idx_t next = fibs[fib_cnt - 2] + fibs[fib_cnt - 1];
        if (next < fibs[fib_cnt - 1])          /* overflow, stop generating */
            break;
        if (next > n)                          /* larger than needed for this n */
            break;

        /* Grow the storage if required. */
        if (fib_cnt == fib_cap) {
            fib_cap *= 2;
            fibs = (idx_t*)realloc(fibs, sizeof(idx_t) * fib_cap);
            if (!fibs) return 0; /* unlikely, but safe */
        }
        fibs[fib_cnt++] = next;
        last_fib = next;
    }

    /* Compute the coefficient a(n). */
    seq_t sum = 0;
    for (size_t i = 0; i < fib_cnt; ++i) {
        idx_t d = fibs[i];
        if (d > n)
            break;
        if (n % d == 0) {
            idx_t m = n / d;
            /* sign = (-1)^(m+1) -> +1 if (m+1) even, -1 otherwise. */
            seq_t sign = ((m + 1) & 1) ? -1 : 1;
            sum += sign * (seq_t)d;
        }
    }
    return sum;
}
