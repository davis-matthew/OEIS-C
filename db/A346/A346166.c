/*
 * A346166  a(n) is the smallest integer k >= 0 such that cosh(k) > 10^(n^2).
 * https://oeis.org/A346166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A346166
 * Formula (OEIS notation):
 *     a(n) = 1 + floor( arccosh(10^(n^2)) )
 * Equivalent exact integer formula used here:
 *     a(n) = 1 + floor( ln(2) + n^2 * ln(10) )
 *
 * The implementation evaluates the floor via high‑precision fixed‑point arithmetic
 * using a scaling factor of 2^60.  ln(10) and ln(2) are represented by the
 * integer parts of their products with 10^18:
 *     ln(10)*10^18 ≈ 2302585092994045684
 *     ln(2) *10^18 ≈ 693147180559945309
 *
 * These constants are multiplied by 2^60 and divided by 10^18 to obtain
 * integer representations of ln(10)*2^60 and ln(2)*2^60.  The product of
 * every term with 2^60 is then integer‑shifted right by 60 bits to recover the
 * required floor.  The resulting value is always exact for any n where it
 * fits into seq_t (which is at least 64‑bit in all supported builds).
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Constant integers: floor( ln(10) * 10^18 ) and floor( ln(2) * 10^18 ) */
    const unsigned long long LN10_18 = 2302585092994045684ULL;   /* ln(10)*10^18 */
    const unsigned long long LN2_18  = 693147180559945309ULL;    /* ln(2)*10^18  */

    const unsigned SHIFT   = 60;                            /* scaling exponent 2^60 */
    const unsigned long long TEN18 = 1000000000000000000ULL;  /* 10^18 */

    /* Convert ln(10) and ln(2) to fixed‑point values multiplied by 2^SHIFT */
    __int128 ln10_scaled = ((__int128)LN10_18 << SHIFT) / TEN18;
    __int128 ln2_scaled  = ((__int128)LN2_18  << SHIFT) / TEN18;

    /* Compute n^2 and then the value n^2*ln10 + ln2 in the scaled domain */
    __int128 nn  = n;
    nn *= nn;                                            /* n^2  (fits in 64 bits) */
    __int128 val = nn * ln10_scaled + ln2_scaled;

    /* Recover the floor of the real expression and add one */
    seq_t result = (seq_t)((val >> SHIFT) + 1);
    return result;
}
