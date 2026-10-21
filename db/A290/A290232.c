/*
 * A290232  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 773", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A290232
 *  a(n) = (10^(n+1)-1)/9            if n is even
 *  a(n) = (10^(n+1)-10^((n+1)/2))/9 if n is odd
 *
 *  The function builds the decimal representation directly to avoid
 *  intermediate overflow for 64‑bit implementations.  For an arbitrary‑precision
 *  build the same construction is exactly correct.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Number of digits in the result */
    idx_t len  = n + 1;
    /* For odd n we need to know how many leading 1’s the number has */
    idx_t half = (n + 1) / 2;

    /* Result is constructed digit by digit starting from the most
       significant digit.  All operations are performed on seq_t so
       the function works for both 64‑bit and arbitrary‑precision
       types. */
    seq_t res = 0;

    if ((n & 1) == 0) {            /* even n – all 1’s */
        for (idx_t i = 0; i < len; ++i) {
            res = res * 10 + 1;
        }
    } else {                       /* odd n – first half 1’s, then zeros */
        for (idx_t i = 0; i < half; ++i) {
            res = res * 10 + 1;
        }
        /* Shift left by half positions: multiply by 10^half.  For
           64‑bit integers the exponent is at most 10 (see analysis);
           for arbitrary precision the helper handles arbitrarily
           large exponents. */
        res *= pow10(half);
    }

    return res;
}
