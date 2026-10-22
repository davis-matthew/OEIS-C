/*
 * A355140  n/d(n) rounded to the nearest integer, where d(n) is the number of
 *          divisors of n (A000005).
 * https://oeis.org/A355140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round (n / A000005(n)) where A000005(n) is the number of
 *            divisors of n. In case of a 0.5 fractional part, ties are
 *            rounded up
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) /* undefined for offset 1; return 0 as safe fallback */
        return 0;
    uint64_t d = divisor_count_u64((uint64_t)n);
    /* integer division and remainder */
    uint64_t q = n / d;
    uint64_t r = n % d;
    /* round to nearest integer, round ties up */
    if (r * 2 >= d)
        q += 1;
    return (seq_t)q;
}
