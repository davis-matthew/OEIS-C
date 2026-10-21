/*
 * A248503  Decimal expansion of molar volume of ideal gas at 0 degrees
 *          Celsius and 1 atm in liters per mole.
 * https://oeis.org/A248503
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimal digit of V_m = (R*T)/P L/mol ; reduced fraction */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Rational constant after reduction:
       V_m = 378515910691426251 / 16887500000000000   ≈ 22.4139695450141... */
    static const uint64_t NUM = 378515910691426251ULL;      /* numerator   */
    static const uint64_t DEN = 16887500000000000ULL;       /* denominator */
    static const uint64_t INT_PART = NUM / DEN;            /* integer part (22) */
    static const uint64_t REM0   = NUM % DEN;               /* initial remainder */

    if (n < 2) return (seq_t)0;                 /* undefined; return 0 */
    if (n == 2) return (seq_t)(INT_PART / 10);  /* first integer digit */
    if (n == 3) return (seq_t)(INT_PART % 10);  /* second integer digit */

    /* n >= 4 : decimal digits */
    idx_t pos = n - 3;           /* 1‑based position in the decimal part */
    uint64_t rem = REM0;         /* current remainder (fits in uint64_t) */

    for (idx_t i = 1; i < pos; ++i) { /* advance remainder to required step */
        rem = rem * 10;
        rem %= DEN;
    }

    rem = rem * 10;
    seq_t digit = (seq_t)(rem / DEN);
    return digit;
}
