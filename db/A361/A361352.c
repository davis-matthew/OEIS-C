/*
 * A361352  Decimal expansion of the conventional value of farad-90 (F_{90}).
 * https://oeis.org/A361352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cons, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimal expansion of the conventional value of farad‑90 (F_{90})
 * a(n) = n‑th digit after the decimal point of the fraction
 *   5521725026748260492435541 / 5521725125000000000000000
 * The function can be called with increasing or arbitrary n.
 * It works for any seq_t (uint8_t … uint64_t, unsigned __int128, or an
 * arbitrary‑precision class).  All arithmetic needed for the digits is
 * performed using unsigned __int128, which is sufficient to hold the
 * numerator and the denominator (they are < 2^128).  The result is cast
 * to seq_t before returning.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper to parse a decimal string into an unsigned __int128. */
static inline unsigned __int128 parse_uint128(const char *s)
{
    unsigned __int128 v = 0;
    for (; *s; ++s)
        v = v * 10 + (unsigned)(*s - '0');
    return v;
}

/* External linkage function required by the build system. */
seq_t a(idx_t n)
{
    /* Numerator and denominator of the farad‑90 fraction
     * stored as unsigned __int128 for safe arithmetic. */
    static bool init = false;
    static unsigned __int128 N = 0;
    static unsigned __int128 D = 0;

    if (!init) {
        N = parse_uint128("5521725026748260492435541");
        D = parse_uint128("5521725125000000000000000");
        init = true;
    }

    /* Compute remainder after n digits */
    unsigned __int128 rem = N % D;  /* N < D, so rem = N */
    for (idx_t i = 0; i < n; ++i)
        rem = (rem * 10) % D;

    /* Get the n-th digit */
    rem = rem * 10;
    unsigned __int128 digit = rem / D;
    return (seq_t)digit;
}
