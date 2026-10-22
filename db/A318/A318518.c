/*
 * A318518  a(n) = A032742(n) AND n-A032742(n), where AND is bitwise-and
 *          (A004198) and A032742 = the largest proper divisor of n.
 * https://oeis.org/A318518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A032742(n) AND (n-A032742(n))
 * Range:     u8 n <= 511, u16 n <= 9885, u32 n <= 9885, u64 n <= 9885,
 *            u128 n <= 9885, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the bitwise AND of the largest proper divisor of n and the difference n–that divisor. */
#include <stdint.h>   /* for uint64_t used in local calculations */

seq_t a(idx_t n)
{
    /* For n = 1, the largest proper divisor is defined as 1,
       so n – 1 = 0 and 1 AND 0 = 0. */
    if (n <= 1)
        return (seq_t)0;

    /* Find the smallest prime factor (spf) of n. */
    idx_t spf = 0;
    for (idx_t i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            spf = i;
            break;
        }
    }
    /* If no divisor was found, n is prime and its spf is itself. */
    if (spf == 0)
        spf = n;

    /* The largest proper divisor is n / spf. */
    seq_t lpd = (seq_t)n / (seq_t)spf;
    seq_t diff = (seq_t)n - lpd;

    return lpd & diff;
}
