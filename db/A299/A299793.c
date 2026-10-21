/*
 * A299793  Decimal expansion of 4*c^2, where c is the speed of light in
 *          vacuum in SI units.
 * https://oeis.org/A299793
 *
 * Offset:    18    (a(18) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   a(n) = (359502071494727056 / 10^(35-n)) % 10, for n ≥ 18
 * Range:     u8 n <= 35, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n outside [18,35] the value is 0. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 18 || n > 35)
        return 0;

    static const idx_t C = 359502071494727056ULL;
    static const idx_t pow10[18] = {
        1ULL,
        10ULL,
        100ULL,
        1000ULL,
        10000ULL,
        100000ULL,
        1000000ULL,
        10000000ULL,
        100000000ULL,
        1000000000ULL,
        10000000000ULL,
        100000000000ULL,
        1000000000000ULL,
        10000000000000ULL,
        100000000000000ULL,
        1000000000000000ULL,
        10000000000000000ULL,
        100000000000000000ULL
    };
    idx_t power = pow10[35 - n];
    idx_t q = C / power;
    seq_t digit = (seq_t)(q % 10ULL);
    return digit;
}
