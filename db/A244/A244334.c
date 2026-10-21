/*
 * A244334  Decimal expansion of 64/169, the upper bound (as given by S.
 *          Finch) of the 2-dimensional simultaneous Diophantine approximation
 *          constant.
 * https://oeis.org/A244334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n‑th decimal digit (0‑based) of the fraction 64/169
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static array to hold one full period of the decimal expansion. */
    static seq_t digits[170];
    static size_t period = 0;

    if (period == 0) {                     /* compute once, lazily */
        seq_t num = 64;
        seq_t den = 169;
        uint64_t rem = (uint64_t)num;          /* remainder < den */
        int seen[170];
        for (int i = 0; i <= 169; ++i) seen[i] = -1;

        size_t pos = 0;
        while (rem != 0 && seen[rem] == -1) {
            seen[rem] = (int)pos;           /* remember position of this remainder */
            rem = rem * 10;
            uint64_t digit = rem / (uint64_t)den;
            digits[pos++] = (seq_t)digit;   /* store the generated digit */
            rem = rem % (uint64_t)den;
        }
        period = pos;                       /* length of the repeating block */
    }

    size_t idx = (size_t)(n % period);
    return digits[idx];
}
