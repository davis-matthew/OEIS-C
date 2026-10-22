/*
 * A384169  a(n) = 4^n * n! * binomial(5*n/4,n) * Sum_{k=1..n} 1/(n+4*k).
 * https://oeis.org/A384169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1..n} (n + 4*i) * ∑_{i=1..n} 1/(n + 4*i)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to a(n) = ∏_{m∈{n+4, n+8, …, 5n}} m  * ∑_{m∈{n+4, n+8, …, 5n}} (1/m) */
/* The product P = ∏ m contains each m once, so each term P/m is an integer. */
/* Thus a(n) = Σ P / m for all m = n+4, n+8, …, 5n. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute P = product of all numbers m = n + 4*i for i=1..n */
    seq_t P = 1;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t m = n + 4 * i;
        P *= (seq_t)m;
    }

    /* sum over P / m */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t m = n + 4 * i;
        sum += P / (seq_t)m;
    }

    return sum;
}
