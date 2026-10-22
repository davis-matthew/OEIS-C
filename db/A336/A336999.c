/*
 * A336999  a(n) = n! * Sum_{d|n} n^d / d!.
 * https://oeis.org/A336999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{d|n} n^d / d! (offset 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1, a(1) is the first term */

    seq_t sum   = 0;
    seq_t fact_n = 1;                /* n!  */
    for (idx_t i = 2; i <= n; ++i) {
        fact_n *= (seq_t)i;
    }

    seq_t pow_n  = 1;                /* current n^d */
    seq_t fact_d = 1;                /* current d!   */

    for (idx_t d = 1; d <= n; ++d) {
        pow_n  *= (seq_t)n;          /* n^d */
        fact_d *= (seq_t)d;           /* d!  */
        if (n % d == 0) {
            seq_t term = fact_n / fact_d;
            term *= pow_n;
            sum += term;
        }
    }
    return sum;
}
