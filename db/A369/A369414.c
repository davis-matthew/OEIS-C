/*
 * A369414  Irregular triangle read by rows: row n lists the values of the
 *          vertices at the n-th level of the MI graph (see comments).
 * https://oeis.org/A369414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 9863, u32 n <= 9863, u64 n <= 9863,
 *            u128 n <= 9863, big any n
 * Helpers:   ilog2_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A369414 – Irregular triangle read by rows
 *
 *  a(0) = 1,  a(1) = 2,  a(2) = 4.
 *  For n ≥ 3 let r = ⌊log₂(n‑1)⌋ + 2      (0‑based row number)
 *  and let k = n – 2^(r‑2)   (1‑based position in that row).
 *  Then   a(n) = 2^r – 3·(k‑1)
 *       = 2^r – 3·(n – 2^(r‑2) – 1).
 *
 *  The function below implements this formula.  All required
 *  types are supplied by the surrounding build system.
 */

#include <stdint.h>
#include "../utils/ilog2_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;

    /* compute row number r (r ≥ 3) */
    unsigned int r = ilog2_u64((uint64_t)n - 1) + 2;

    /* 2^r and 2^(r‑2) as seq_t values (shifts done on int) */
    seq_t pow2    = (seq_t)(1 << r);
    seq_t pow_half = (seq_t)(1 << (r - 2));

    /* a(n) = pow2 – 3 * (n – pow_half – 1) */
    seq_t val = pow2 - (seq_t)3 * ((seq_t)n - pow_half - 1);
    return val;
}
