/*
 * A294701  Number of permutations of [n] avoiding {4231, 2143, 1342}.
 * https://oeis.org/A294701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (28 - 32*2^n + 22*n + 7*2^n*n + 2*n^2)/4 (for n > 1)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Pre‑defined small values */
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 6;

    /* Use signed 128‑bit for intermediate sign handling */
    __int128 nn = n;

    /* Compute part1 = (28 + 22*n + 2*n^2) / 4  (exact integer) */
    seq_t part1 = ( (seq_t)28 + (seq_t)22 * nn + (seq_t)2 * nn * nn ) / 4;

    /* Compute part2 = (7*n - 32) * 2^(n-2)  (signed) */
    seq_t pw   = pow2(n - 2);            /* 2^(n-2) */
    __int128 coeff = 7 * nn - 32;        /* signed coefficient */

    /* abscoeff fits in seq_t for n up to 10'000 (and larger for any int_t) */
    seq_t abscoeff = coeff >= 0 ? (seq_t)coeff : (seq_t)(-coeff);

    seq_t part2_abs = pw * abscoeff;     /* unsigned product */

    seq_t result;
    if (coeff >= 0)
        result = part1 + part2_abs;
    else
        result = part1 - part2_abs;

    return result;
}
