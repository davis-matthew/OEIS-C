/*
 * A317276  a(n) = Sum_{k=0..n} binomial(n-1,k-1)*binomial(2*k,k)*n!/(k + 1)!.
 * https://oeis.org/A317276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n-1,k-1) * binomial(2*k,k) *
 *            n!/(k+1)!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS sequence A317276. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* binomial(2k,k) for k = 0..n */
    seq_t *b2k = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!b2k) return (seq_t)0;
    b2k[0] = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        b2k[k] = b2k[k-1] * (seq_t)(2*k) * (seq_t)(2*k - 1) / (seq_t)(k * k);
    }

    /* binomial(n-1,k-1) for k = 1..n-1 */
    seq_t *bn1 = NULL;
    if (n > 1) {
        bn1 = (seq_t*)malloc((n) * sizeof(seq_t));          /* indices 1..n‑1 used */
        if (!bn1) { free(b2k); return (seq_t)0; }
        bn1[1] = (seq_t)1;
        for (idx_t k = 2; k <= n-1; ++k) {
            bn1[k] = bn1[k-1] * (seq_t)(n - k + 1) / (seq_t)(k - 1);
        }
    }

    seq_t sum = (seq_t)0;
    seq_t prod = (seq_t)1;                 /* product_{j=k+2..n} j, starts empty for k=n-1 */

    for (idx_t k = n-1; k >= 1; --k) {
        seq_t term = bn1[k] * b2k[k];
        term = term * prod;
        sum += term;
        if (k > 1) prod = prod * (seq_t)(k + 1);
    }

    /* term for k = n: binom(n-1,n-1) = 1 */
    seq_t term_n = b2k[n] / (seq_t)(n + 1);
    sum += term_n;

    free(b2k);
    if (bn1) free(bn1);
    return sum;
}
