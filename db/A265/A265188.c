/*
 * A265188  Nonnegative m for which 3*floor(m^2/11) = floor(3*m^2/11).
 * https://oeis.org/A265188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   m ≡ 0, 1, 5, 6, 10 (mod 11). Hence a(n) = 11 * ⌊(n‑1)/5⌋ +
 *            offset[(n‑1) mod 5], where offset = {0, 1, 5, 6, 10}. This
 *            yields the ascending sequence of all non‑negative m satisfying
 *            3*⌊m²/11⌋ = ⌊3m²/11⌋
 * Range:     u8 n <= 117, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t used in constants */

static const idx_t offsets[5] = {0ULL, 1ULL, 5ULL, 6ULL, 10ULL};

seq_t a(idx_t n)
{
    /* offset index (0‑based) and block count */
    idx_t q = (n - 1) / 5;
    idx_t r = (n - 1) % 5;

    /* compute result in seq_t */
    seq_t val = ((seq_t)q) * (seq_t)11 + (seq_t)offsets[r];
    return val;
}
