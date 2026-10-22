/*
 * A339915  Number of divisors of n with the same number of decimal digits as
 *          n.
 * https://oeis.org/A339915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{d|n} [floor(log_10(d)) = floor(log_10(n))], i.e.
 *            count of divisors of n that have the same number of decimal
 *            digits as n. This implementation enumerates the divisors via a
 *            simple loop, which runs in O(sqrt(n)) time and uses only integer
 *            arithmetic
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute number of decimal digits of n. */
    int digits = 0;
    idx_t t = n;
    do {
        ++digits;
        t /= 10;
    } while (t);

    /* Compute lower bound: 10^(digits-1). */
    idx_t lower = 1;
    for (int i = 1; i < digits; ++i) {
        lower *= 10;
    }

    /* Compute upper bound: min(10^digits-1, n).  For digits==20 the
       value 10^digits would overflow 64‑bit, but any divisor of n
       is <= n, so we use n as the upper bound. */
    idx_t upper;
    if (digits == 20) {
        upper = n;
    } else {
        upper = lower * 10 - 1;
    }

    /* Count the divisors within [lower, upper]. */
    seq_t count = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            if (d1 >= lower && d1 <= upper) {
                ++count;
            }
            idx_t d2 = n / i;
            if (d2 != d1 && d2 >= lower && d2 <= upper) {
                ++count;
            }
        }
    }

    return count;
}
