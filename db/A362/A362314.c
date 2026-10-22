/*
 * A362314  a(n) = n! * Sum_{k=0..floor(n/4)} (n/4)^k /(k! * (n-4*k)!).
 * https://oeis.org/A362314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n! * Sum_{k=0..floor(n/4)} (n/4)^k /(k! * (n-4*k)!).
 * The implementation uses the recurrence
 *   term_0 = 1
 *   term_k = term_{k-1} * (n-4k+1)(n-4k+2)(n-4k+3)(n-4k+4) * n / (4k)
 *   a(n) = sum_{k=0}^{floor(n/4)} term_k
 *
 * The code relies only on integer arithmetic; no floating point or external libraries.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Fast path for trivial cases */
    if (n == 0) return 1;
    if (n < 4) return 1;          /* floor(n/4) == 0 */

    seq_t sum   = 1;          /* term_0 = 1 */
    seq_t term  = 1;          /* current term_k */
    idx_t maxk  = n / 4;      /* floor(n/4) */

    for (idx_t k = 1; k <= maxk; ++k) {
        /* Compute the numerator factors as seq_t values */
        seq_t a1 = (seq_t)(n - 4*k + 1);
        seq_t a2 = (seq_t)(n - 4*k + 2);
        seq_t a3 = (seq_t)(n - 4*k + 3);
        seq_t a4 = (seq_t)(n - 4*k + 4);
        seq_t num = a1 * a2;
        num = num * a3;
        num = num * a4;
        num = num * (seq_t)n;                  /* multiply by n */

        seq_t denom = (seq_t)(4*k);

        /* Update the current term */
        term = term * num / denom;

        /* Add to the running sum */
        sum += term;
    }
    return sum;
}
