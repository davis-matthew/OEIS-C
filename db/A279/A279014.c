/*
 * A279014  a(n) = Sum_{k=0..n} fibonacci(k+1)*binomial(2*n-1,n-k).
 * https://oeis.org/A279014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} fibonacci(k+1)*binomial(2*n-1,n-k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* Pre‑compute binomial coefficient C(2*n-1, n) */
    idx_t N = 2 * n - 1;
    seq_t binom = 1;
    for (idx_t i = 1; i <= n; ++i) {
        binom = binom * (seq_t)(2 * n - i) / i;   /* exact division */
    }

    /* Fibonacci numbers up to F_{n+1} (F_1 = 1, F_2 = 1) */
    seq_t *fib = (seq_t *)malloc((n + 2) * sizeof(seq_t));
    if (!fib) return 0;                       /* out of memory */
    fib[1] = 1;
    fib[2] = 1;
    for (idx_t i = 3; i <= n + 1; ++i) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }

    seq_t sum = 0;
    idx_t j = n;           /* current j in C(N, j) */
    for (idx_t k = 0; k <= n; ++k) {
        sum += binom * fib[k + 1];
        if (k == n) break;
        /* Update binomial coefficient for j-1 */
        binom = binom * j / (N - j + 1);
        --j;
    }

    free(fib);
    return sum;
}
