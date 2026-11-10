/*
 * A377246  a(n) = (n!^2*n^(n-1)/4) * Sum_{k=4..n} A000276(k) / (n^k *
 *          (n-k)!).
 * https://oeis.org/A377246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!^2*n^(n-1)/4) * Sum_{k=4..n} A000276(k) / (n^k *
 *            (n-k)!), * computed as Sum_{k=4..n} (n!/4) * (n!/(n-k)!) *
 *            n^(n-1-k) * A000276(k) * (the k = n term is (n!/4) * (n-1)! *
 *            A000276(n)); a(1) = a(2) = a(3) = 0. * A000276(k) by f(k+1) =
 *            k*(f(k) + (k-2)!), f(4) = 3
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t i, k;
    seq_t fact, P, fn1, f, g, ff, sum;

    if (n < 4) return (seq_t)0;

    fact = 1;                       /* n! */
    for (i = 2; i <= n; i++) fact *= (seq_t)i;
    P = fact / 4;                   /* n!/4, an integer for n >= 4 */
    fn1 = fact / (seq_t)n;          /* (n-1)! */

    f = 3;                          /* A000276(k), k = 4 */
    g = 2;                          /* (k-2)! */
    ff = (seq_t)n * (seq_t)(n - 1) * (seq_t)(n - 2); /* n!/(n-3)! */
    sum = 0;
    for (k = 4; k <= n; k++) {
        seq_t term;
        ff *= (seq_t)(n - k + 1);   /* n!/(n-k)! */
        if (k == n) {
            term = P * fn1 * f;
        } else {
            seq_t pw = 1;
            for (i = 0; i < n - 1 - k; i++) pw *= (seq_t)n;
            term = P * ff * pw * f;
        }
        sum += term;
        f = (f + g) * (seq_t)k;     /* A000276(k+1) */
        g *= (seq_t)(k - 1);        /* (k-1)! */
    }
    return sum;
}
