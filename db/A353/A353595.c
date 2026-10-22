/*
 * A353595  Array read by ascending antidiagonals. Generalized Fibonacci
 *          numbers F(n, k) = (psi^(k - 1)*(phi + n) - phi^(k - 1)*(psi + n))
 *          / (psi - phi) where phi = (1+sqrt(5))/2 and psi = (1-sqrt(5))/2.
 *          F(n, k) for n >= 0 and k >= 0.
 * https://oeis.org/A353595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = F(d‑t, t) where d = floor((sqrt(8n+1)-1)/2) (the
 *            antidiagonal number), t = n – d*(d+1)/2 (offset on that
 *            diagonal), and F(i,j) = i*fib(j‑1) + fib(j). fib(0)=0, fib(1)=1,
 *            fib(–1)=1 (handled via j==0)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */

/* Compute the ordinary Fibonacci number fib(k) for k ≥ 0.
   Uses only integer additions. */
static seq_t fib_small(idx_t k) {
    if (k == 0) return (seq_t)0;
    if (k == 1) return (seq_t)1;
    seq_t a = (seq_t)0;
    seq_t b = (seq_t)1;
    for (idx_t i = 2; i <= k; ++i) {
        seq_t tmp = a + b;
        a = b;
        b = tmp;
    }
    return b;
}

seq_t a(idx_t n)
{
    /* Find the diagonal number d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t d = 0;
    for (;;) {
        unsigned __int128 val = (unsigned __int128)(d + 1) * (d + 2) / 2;
        if (val > n) break;
        ++d;
    }

    /* First index on diagonal d */
    unsigned __int128 first_big = (unsigned __int128)d * (d + 1) / 2;
    idx_t first = (idx_t)first_big;

    /* Offset within the diagonal */
    idx_t t = n - first;

    /* Coordinates (i, j) on the antidiagonal */
    idx_t i = d - t;
    idx_t j = t;

    /* Compute F(i, j) = i*fib(j‑1) + fib(j) */
    seq_t fj, fjm1;
    if (j == 0) {
        fj   = (seq_t)0;   /* fib(0) */
        fjm1 = (seq_t)1;   /* fib(–1) */
    } else {
        fj   = fib_small(j);
        fjm1 = fib_small(j - 1);
    }

    return (seq_t)i * fjm1 + fj;
}
