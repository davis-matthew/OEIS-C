/*
 * A333097  a(n) = the n-th order Taylor polynomial (centered at 0) of
 *          c(x)^(5*n) evaluated at x = 1, where c(x) = (1 - sqrt(1 -
 *          4*x))/(2*x) is the o.g.f. of the sequence of Catalan numbers
 *          A000108.
 * https://oeis.org/A333097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} 5*n/(5*n+2*k)*binomial(5*n+2*k, k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* with a(0) = 1. */

#include <stdint.h>
#include <stdbool.h>

/* Compute the n-th term of OEIS A333097. */
seq_t a(idx_t n)
{
    /* Special case: a(0) = 1. */
    if (n == 0) return (seq_t)1;

    /* 5*n as seq_t. */
    seq_t five_n = (seq_t)5 * (seq_t)n;

    /* B = binomial(5*n + 2*k, k), starting with k=0 (B=1). */
    seq_t B = (seq_t)1;

    /* Initial term for k = 0: 5*n/(5*n) * 1 = 1. */
    seq_t sum = (seq_t)1;

    /* Loop over k = 1..n. */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* Compute B_k from B_{k-1} using the recurrence:
           B_k = B_{k-1} * (5*n + 2*k) * (5*n + 2*k - 1)
                               / ( k * (5*n + k) ). */
        unsigned __int128 num1 = (unsigned __int128)(five_n + 2*k);
        unsigned __int128 num2 = (unsigned __int128)(five_n + 2*k - 1);
        unsigned __int128 den1  = (unsigned __int128)k;
        unsigned __int128 den2  = (unsigned __int128)(five_n + k);

        unsigned __int128 prod = (unsigned __int128)B * num1 * num2;
        seq_t B_next = (seq_t)(prod / (den1 * den2));
        B = B_next;

        /* Term for this k:
           term = 5*n * B_k / (5*n + 2*k). */
        unsigned __int128 term_num = (unsigned __int128)five_n * (unsigned __int128)B;
        unsigned __int128 term_den = (unsigned __int128)(five_n + 2*k);
        seq_t term = (seq_t)(term_num / term_den);

        sum += term;
    }

    return sum;
}
