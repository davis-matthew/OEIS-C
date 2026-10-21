/*
 * A243002  a(n) = date of Easter in April of n, in the Gregorian calendar
 *          after A.D. 1582. a(n) = 0 if Easter occurs in March.
 * https://oeis.org/A243002
 *
 * Offset:    1583    (a(1583) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1658, u32 n <= 1658, u64 n <= 1658,
 *            u128 n <= 1658, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Computes Gregorian Easter date (Meeus/Jones/Butcher)
 * Returns the day in April or 0 if Easter falls in March. */
seq_t a(idx_t n)
{
    /* Cast idx_t to seq_t. All intermediate steps are in seq_t. */
    seq_t year = (seq_t)n;

    seq_t a1 = year % 19;
    seq_t b  = year / 100;
    seq_t c  = year % 100;
    seq_t d  = b / 4;
    seq_t e  = b % 4;
    seq_t f  = (b + 8) / 25;
    seq_t g  = (b - f + 1) / 3;
    seq_t h  = (19 * a1 + b - d - g + 15) % 30;
    seq_t i  = c / 4;
    seq_t k  = c % 4;
    seq_t l  = (32 + 2 * e + 2 * i - h - k) % 7;
    seq_t m  = (a1 + 11 * h + 22 * l) / 451;

    seq_t month = (h + l - 7 * m + 114) / 31;
    if (month != 4) {
        return 0;                      /* Easter is in March */
    }

    seq_t day = ((h + l - 7 * m + 114) % 31) + 1;
    return day;                         /* Day in April */
}
