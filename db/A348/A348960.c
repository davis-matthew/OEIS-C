/*
 * A348960  a(n) = floor(log_10(Pi*n!)).
 * https://oeis.org/A348960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊log₁₀(π · n!)⌋ (offset 0)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

static const long double LOG10_PI =
    0.497149872694133854352089001711147L; /* log10(π) */

seq_t a(idx_t n)
{
    /* Sum of log10(k) for k = 1 .. n   (n = 0 or 1 gives sum = 0) */
    long double sum = 0.0L;
    for (idx_t k = 2; k <= n; ++k) {
        sum += log10l((long double)k);
    }

    /* floor( sum + log10(π) )  --> integer result */
    unsigned long long val =
        (unsigned long long)floorl(sum + LOG10_PI);

    return (seq_t)val;           /* cast to seq_t (built‑in or big integer) */
}
