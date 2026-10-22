/*
 * A324032  Decimal expansion of the conversion factor from the old SI K
 *          (kelvin) to the combination of 2019 SI units h*Δν_{Cs}/k.
 * https://oeis.org/A324032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A324032
    a(n) is the n-th digit (after the decimal point) of
    1.380649/(6.62607015*9192631770)*1e11
    i.e. the decimal expansion of
          2.2666652646011048673601081473620663569743671804663930…  */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 128‑bit constants for the exact rational numerator and denominator */
    static const unsigned __int128 N128 =
        (unsigned __int128)1380649 *
        (unsigned __int128)100000000 *
        (unsigned __int128)100000000000ULL;     /* 10^11 */

    static const unsigned __int128 D128 =
        (unsigned __int128)1000000 *
        (unsigned __int128)662607015 *
        (unsigned __int128)9192631770ULL;

    /* integer part of the number (2 for this constant) */
    static const seq_t INT_PART = (seq_t)(N128 / D128);  /* 2 */

    if (n == 1)            /* first digit (integer part) */
        return INT_PART;

    /* generate decimal digits one by one */
    idx_t pos = n - 1;     /* number of decimal digits to produce */
    unsigned __int128 rem = N128 % D128;
    seq_t digit = 0;

    for (idx_t i = 0; i < pos; ++i) {
        rem *= 10;
        digit = (seq_t)(rem / D128);
        rem %= D128;
    }

    return digit;
}
