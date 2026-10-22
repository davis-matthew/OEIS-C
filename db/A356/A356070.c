/*
 * A356070  Number of astronomical units (A163103) in a light-year (A213614).
 * https://oeis.org/A356070
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the (n‑5)th digit (0‑based) of the decimal expansion of
 *  the rational number 431996825232/6830953 (i.e. the ratio
 *  A213614/A163103).  The first term a(5) is the most significant
 *  digit of the integer part 63241.  The sequence continues with
 *  the digits of the fractional part, repeating after period 568524.
 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* constants of the decimal fraction */
    const seq_t NUM = (seq_t)431996825232ULL;
    const seq_t DEN = (seq_t)6830953ULL;

    static seq_t int_part = 0;
    static seq_t rem      = 0;
    static idx_t int_digits = 0;
    static bool initialized = false;

    if (!initialized) {
        int_part = NUM / DEN;
        rem      = NUM % DEN;

        /* number of decimal digits in the integer part */
        seq_t t = int_part;
        int_digits = 0;
        while (t > (seq_t)0) {
            t = t / (seq_t)10;
            int_digits++;
        }
        initialized = true;
    }

    if (n < 5)            /* values below the offset are unused */
        return 0;

    idx_t pos = n - 5;    /* 0‑based position in the infinite digit string */

    if (pos < int_digits) {
        /* digit from the integer part */
        idx_t shift = int_digits - pos - 1;   /* power of 10 to divide by */
        seq_t divisor = 1;
        for (idx_t i = 0; i < shift; ++i)
            divisor = divisor * (seq_t)10;
        seq_t digit = (int_part / divisor) % (seq_t)10;
        return digit;
    }

    /* digit from the fractional part */
    idx_t frac_pos = pos - int_digits;   /* 0‑based fractional index */
    seq_t r = rem;
    for (idx_t i = 0; i < frac_pos; ++i) {
        r = (r * (seq_t)10) % DEN;
    }
    seq_t digit = (r * (seq_t)10) / DEN;
    return digit;
}
