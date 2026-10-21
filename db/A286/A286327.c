/*
 * A286327  Least possible sum of the squares of the two initial terms of a
 *          Fibonacci-like sequence containing n.
 * https://oeis.org/A286327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) is the least possible sum of the squares of the two initial
 *            * terms of a Fibonacci‑like sequence that contains the integer
 *            n. * The function computes this via an exhaustive search over
 *            all possible * Fibonacci transfer indices m and all non‑negative
 *            integer solutions * (x, y) of the linear D...
 * Range:     u8 n <= 15, u16 n <= 1226, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Treat n == 0 directly. */
    if (n == 0) {
        return (seq_t)0;
    }

    seq_t N = (seq_t)n;            /* Work in seq_t arithmetic. */

    /* Initialise answer with the trivial representation n = n·1 + 0·0
     * (from m = 0 or m = 1).  This guarantees a finite initial value. */
    seq_t best = N * N;            /* x = n, y = 0. */

    /* Generate Fibonacci numbers up to the first one exceeding N.
     * For simplicity we allocate a fixed array that is large enough
     * for the problem constraints: for n <= 10 000 only the first
     * 30 Fibonacci numbers are needed. */
    seq_t fib[100];
    size_t fsz = 2;
    fib[0] = (seq_t)0;
    fib[1] = (seq_t)1;

    /* Extend the Fibonacci sequence while the last element is ≤ N. */
    while (fib[fsz - 1] <= N) {
        fib[fsz] = fib[fsz - 1] + fib[fsz - 2];
        ++fsz;
        if (fsz >= sizeof(fib) / sizeof(fib[0])) {
            break; /* Safety guard: should not happen for n <= 10 000. */
        }
    }

    /* For each possible transfer index m ≥ 2, examine all y such that
     * 0 ≤ y ≤ N / F_{m-1}.  For each valid (x, y) pair compute
     * x² + y² and update the minimum. */
    for (size_t i = 2; i < fsz; ++i) {
        seq_t Fm  = fib[i];      /* F_m   */
        seq_t Fm1 = fib[i - 1];  /* F_{m-1} */

        /* F_{m-1} is guaranteed > 0 for i ≥ 2. */
        seq_t max_y = N / Fm1;   /* Floor division. */

        for (seq_t y = 0; y <= max_y; ++y) {
            seq_t rem = N - y * Fm1;
            if (rem % Fm == 0) {
                seq_t x = rem / Fm;
                seq_t sum = x * x + y * y;
                if (sum < best) {
                    best = sum;
                }
            }
        }
    }

    return best;
}
