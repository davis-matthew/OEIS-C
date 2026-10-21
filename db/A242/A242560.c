/*
 * A242560  Least positive number k such that (n!-k)/(n-k) is an integer.
 * https://oeis.org/A242560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = n - n / spf(n), where spf(n) is the smallest prime factor
 *            of n. For n = 1 the definition from the OEIS entry gives a(1) =
 *            2
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are provided by the build system; no definition here. */

seq_t a(idx_t n)
{
    /* offset: a(1) = 2 */
    if (n == 1)
        return (seq_t)2;

    /* n >= 2: find the smallest prime factor spf of n. */
    idx_t spf;
    if ((n & 1ULL) == 0) {
        spf = 2;
    } else {
        /* trial division by odd numbers up to sqrt(n). */
        spf = n;                     /* default: n is prime */
        for (idx_t i = 3; i * i <= n; i += 2) {
            if (n % i == 0) {
                spf = i;
                break;
            }
        }
    }

    /* largest proper divisor d = n / spf.  Then a(n) = n - d. */
    idx_t d = n / spf;
    return (seq_t)(n - d);
}
