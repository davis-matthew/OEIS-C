/*
 * A376952  a(n) = least k such that (n*Pi/2)^(2k)/(2 k)! < 1.
 * https://oeis.org/A376952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that ((n*Pi/2)^(2k))/(2k)! < 1
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, for n>0 use the inequality above */
#include <math.h>
#include <stdint.h>

/* external types supplied by the build system */
seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0)
        return (seq_t)1;

    /* Pi as a long double constant */
    static const long double PI = 3.141592653589793238462643383279502884L;

    /* Pre‑compute log(x) where x = n*Pi/2 */
    long double x  = ((long double)n) * PI / 2.0L;
    long double logx = logl(x);            /* ln(x) */

    /* log_term holds ln(term_k) = ln( (x^(2k)) / (2k)! ) */
    long double log_term = 0.0L;           /* for k = 0, term_0 = 1 => ln(1) = 0 */

    idx_t k = 0;
    for (;; ++k)
    {
        /* Update log_term to the next k (k > 0) using the recurrence
           term_k = term_{k-1} * x^2 / ((2k-1)(2k))   */
        if (k > 0)
        {
            log_term += 2.0L * logx
                        - logl((long double)(2 * k - 1))
                        - logl((long double)(2 * k));
        }

        /* term_k < 1  <=>  log_term < 0  */
        if (log_term < 0.0L)
            return (seq_t)k;
    }
}
