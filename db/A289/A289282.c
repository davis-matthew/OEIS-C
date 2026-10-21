/*
 * A289282  The least significant four bytes of n! interpreted in two's
 *          complement.
 * https://oeis.org/A289282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: least significant 32 bits of n! interpreted as a signed 32‑bit
 * two's complement integer.  For n ≥ 34 the factorial contains the factor
 * 2³², so the least significant 4 bytes are all zero and the sequence term
 * is 0.
 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* For n ≥ 34 the result is zero */
    if (n >= 34u)
        return (seq_t)0;

    /* Pre‑compute the factorial modulo 2³² up to 33 once */
    static uint32_t fact[34];
    static bool init = false;
    if (!init) {
        fact[0] = 1u;
        for (idx_t i = 1; i <= 33; ++i)
            fact[i] = (uint32_t)((uint64_t)fact[i - 1] * (uint64_t)i);
        init = true;
    }

    uint32_t low32 = fact[(idx_t)n];     /* least significant 4 bytes */
    seq_t   result = (seq_t)(int32_t)low32; /* two's complement signed */
    return result;
}
