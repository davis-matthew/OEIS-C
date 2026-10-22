/*
 * A392064  Decimal expansion of the expected number of consecutive random
 *          decimal digits whose running sum first exceeds 9.
 * https://oeis.org/A392064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(1) = floor((10/9)^10) = 2, a(n) for n >= 2 is the (n‑1)th
 *            digit after the decimal point of (10/9)^10. It can be computed
 *            as: d = floor( 10^(n-1) * 10^10 / 3^20 ) mod 10 which equals
 *            floor( r / 3^20 ) where r = 10^(n+9) mod (10 * 3^20)
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the integer part separately */
    if (n == 0) return 0;                  /* out of bounds, return 0 */
    if (n == 1) return (seq_t)(10000000000ULL / 3486784401ULL);

    /* Constants for the rational (10/9)^10 = 10^10 / 3^20 */
    const uint64_t D = 3486784401ULL;          /* 3^20 */
    const uint64_t M = 10ULL * D;              /* 10 * D */

    /* Exponent for modular power: 10^(n+9) mod M */
    idx_t exp = n + 9;

    uint64_t base = 10ULL % M;
    uint64_t result = 1ULL % M;

    /* Binary modular exponentiation using 128-bit multiplication to avoid overflow */
    while (exp) {
        if (exp & 1ULL) {
            unsigned __int128 mul = (unsigned __int128)result * base;
            result = (uint64_t)(mul % M);
        }
        unsigned __int128 mul = (unsigned __int128)base * base;
        base = (uint64_t)(mul % M);
        exp >>= 1;
    }

    /* The required digit is floor(result / D) */
    uint64_t digit = result / D;
    return (seq_t)digit;
}
