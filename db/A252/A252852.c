/*
 * A252852  Decimal expansion of the Boltzmann constant in Hz/K according to
 *          the 2019 SI system.
 * https://oeis.org/A252852
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) yields the nth decimal digit (starting at n = 11) of the
 *            constant 20836619123.32757298..., i.e. the decimal expansion of
 *            the Boltzmann constant in Hz/K (2019 SI)
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Constant values (fit in 64 bits). */
    const uint64_t INT_PART   = 20836619123ULL; /* integer part  */
    const uint64_t NUM_FRAC   = 43410431ULL;    /* fractional part   */
    const uint64_t DEN_FRAC   = 132521403ULL;   /* denominator       */

    /* Convert the 1‑based OEIS index to a 0‑based position in the digit
       string.  The first digit (2) is at n = 11, so position 0.  */
    idx_t pos = n - 11;

    /* ---------- Integer part (first 11 digits) ---------- */
    if (pos < 11) {
        /* Compute 10^(10 - pos) using 64‑bit integer arithmetic. */
        uint64_t power = 1;
        for (idx_t i = 0; i < (10 - pos); ++i)
            power *= 10ULL;

        uint64_t digit = (INT_PART / power) % 10ULL;
        return (seq_t)digit;
    }

    /* ---------- Decimal (fractional) part ----------- */
    idx_t frac_pos = pos - 11;               /* zero‑based digit after '.' */
    uint64_t rem = NUM_FRAC;                 /* current remainder      */
    uint64_t digit = 0;

    for (idx_t i = 0; i <= frac_pos; ++i) {
        rem *= 10ULL;
        digit = rem / DEN_FRAC;
        rem   %= DEN_FRAC;
    }

    return (seq_t)digit;
}
