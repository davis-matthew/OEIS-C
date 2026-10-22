/*
 * A367821  a(n) = first exponent k such that n^k starts with 9 or -1 if n is
 *          a power of 10.
 * https://oeis.org/A367821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, base
 * Formula:   a(n) = minimal k such that n^k starts with 9, or -1 if n is a
 *            power of 10
 * Range:     i8 n <= 5, i16 n <= 71, i32 n <= 71, i64 n <= 71, i128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <math.h>

static bool is_power_of_10(idx_t n)
{
    if (n == 0) return false;
    while (n % 10 == 0) n /= 10;
    return n == 1;
}

seq_t a(idx_t n)
{
    if (is_power_of_10(n)) return (seq_t)-1;

    const long double LOG10_9 = log10l(9.0L);
    const long double EPS     = 1e-15L;
    long double alpha = log10l((long double)n);

    idx_t k = 1;
    while (1) {
        long double f = (long double)k * alpha;
        long double frac = f - floorl(f);
        if (frac + EPS >= LOG10_9) break;
        ++k;
    }
    return (seq_t)k;
}
