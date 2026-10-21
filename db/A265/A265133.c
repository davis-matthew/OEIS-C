/*
 * A265133  Beatty sequence for log(2).
 * https://oeis.org/A265133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*log(2))
 * Range:     u8 n <= 369, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const long double LOG2_CONST = 0.6931471805599453094172321214581765680755001343602552L;

seq_t a(idx_t n)
{
    /* use long double for sufficient precision, then truncate to integer */
    uint64_t v = (uint64_t)((long double)n * LOG2_CONST);
    return (seq_t)v;
}
