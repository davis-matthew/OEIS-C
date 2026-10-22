/*
 * A378142  a(n) = n + floor(n*s/r) + floor(n*t/r), where r=2^(1/4),
 *          s=2^(1/2), t=2^(3/4).
 * https://oeis.org/A378142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n·2^(1/4)⌋ + ⌊n·√2⌋
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   floor_n_sqrt2, floor_n_w
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the floor of n times 2^(1/4) and of n times √2 without using floating point */
/* Uses 128‑bit arithmetic for intermediates – sufficient for n up to millions         */

#include <stdint.h>
#include "../utils/floor_n_sqrt2.h"
#include "../utils/floor_n_w.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    return nseq + floor_n_w(n) + floor_n_sqrt2(n);
}
