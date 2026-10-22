/*
 * A342486  Decimal expansion of number of kilograms (kg) in the mass
 *          equivalent of 1 electronvolt (eV/c^2).
 * https://oeis.org/A342486
 *
 * Offset:    -35    (a(-35) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth digit of the decimal coefficient 1.7826619… obtained
 *            from 1602176634·10⁻²⁸ / (299792458²), offset –35 (a(–35) is the
 *            first digit 1)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint64_t NUM = 160217663400000000ULL; /* 1602176634·10⁸  */
static const uint64_t DEN = 89875517873681764ULL;  /* 299792458²     */

seq_t a(idx_t n)
{
    if (n < -35) return (seq_t)0;          /* undefined for indices below the offset */

    idx_t pos = n + 35;                     /* 0‑based position of the required digit */

    uint64_t rem   = NUM % DEN;             /* remainder after integer part */
    uint64_t digit = NUM / DEN;             /* integer part (digit 1) */

    if (pos == 0) return (seq_t)digit;

    for (idx_t i = 1; i <= pos; ++i) {
        rem   *= 10ULL;                     /* shift one decimal place */
        digit  = rem / DEN;                 /* next decimal digit */
        rem   %= DEN;                       /* update remainder */
        if (i == pos) return (seq_t)digit; /* desired digit found */
    }

    return (seq_t)0; /* unreachable, but satisfies compiler */
}
