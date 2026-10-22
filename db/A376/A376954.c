/*
 * A376954  a(n) = least k such that (2n*Pi/3)^(2k)/(2 k)! < 1.
 * https://oeis.org/A376954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k such that (2*n*Pi/3)^(2k)/(2k)! < 1
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm iteratively computes the logarithm of the term
 * to avoid overflow: log(term_k) = 2k*log(x) - log((2k)!).
 * Using the recurrence
 *     log_term_k = log_term_{k-1} + 2*log(x) - log(2k-1) - log(2k)
 * we stop when log_term_k < 0.  For n = 0 the first term is already < 1,
 * so a(0) = 1.  All arithmetic on seq_t is done exactly, while
 * long double is used only for temporary floating‑point calculations
 * that do not affect the final integer result. */

#include <math.h>

static const long double PI = 3.141592653589793238462643383279502884L;

seq_t a(idx_t n)
{
    /* n may be 0 */
    if (n == 0)
        return (seq_t)1;   /* (2*0*Pi/3)^(0) / 0! == 1, next term already < 1 */

    /* x = 2*n*Pi/3  (long double for precision) */
    long double x = (long double)2 * (long double)n * PI / (long double)3;
    long double logx = logl(x);          /* log(x) */

    long double log_term = 0.0L;          /* log(term_0) = log(1) = 0 */

    for (idx_t k = 1; ; ++k)
    {
        /* Update log_term for k:
         * log(term_k) = log(term_{k-1}) + 2*log(x) - log(2k-1) - log(2k)
         */
        log_term += 2.0L * logx
                  - logl((long double)(2 * k - 1))
                  - logl((long double)(2 * k));

        if (log_term < 0.0L)                 /* term_k < 1  */
            return (seq_t)k;
    }
}
