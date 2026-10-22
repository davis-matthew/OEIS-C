/*
 * A375666  Number of seconds in time units: second, minute, hour, day, week,
 *          month, year, decade, century, millennium, eon (in Julian years).
 * https://oeis.org/A375666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, nonn, full
 * Formula:   Number of seconds in successive time units: a(1)=1 // second
 *            a(2)=60 // minute a(3)=3600 // hour a(4)=86400 // day
 *            a(5)=604800 // week a(6)=2629800 // month (average 30.4375 days)
 *            a(7)=31557600 // year (average Julian year 365.25 days)
 *            a(8)=a(7)*10 // decade = 10 years a(9)=a(8)*10 // century = 1...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 9, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The offset is 1: a(1) is defined.  For n<1 the sequence is not defined. */
    if (n == 0) return 0;

    /* Basic units up to a month */
    if (n == 1) return 1;
    if (n == 2) return 60;                   // 60 s
    if (n == 3) return 60 * 60;              // 3600 s
    if (n == 4) return 60 * 60 * 24;         // 86400 s
    if (n == 5) return 60 * 60 * 24 * 7;     // 604800 s
    if (n == 6) return 2629800;              // 86400 * 487/16

    /* Years and longer units */
    const seq_t year = 31557600; /* seconds in an average Julian year */
    switch (n) {
        case 7:  return year;                     // year
        case 8:  return year * 10;                // decade
        case 9:  return year * 100;               // century
        case 10: return year * 1000;              // millennium
        case 11: return year * 1000000000;        // eon (1e9 years)
        default: return 0;                        /* undefined for n>11 */
    }
}
