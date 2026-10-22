/*
 * A384192  Complement of A384191.
 * https://oeis.org/A384192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 3 * ⌊ ⌊ k·φ ⌋ ⌋, where φ = (1+√5)/2 and k = ⌈ n/3 ⌉.
 *            The inner floor can be computed as floor( (k + floor(k·√5)) / 2
 *            ). Here floor_sqrt returns ⌊√x⌋ for integer x
 * Range:     u8 n <= 9, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute k = ceil(n/3) = (n-1)/3 + 1 (all are unsigned) */
    idx_t k = (n - 1) / 3 + 1;
    seq_t kk = (seq_t)k;

    /* t = ⌊k · √5⌋  computed as ⌊√(5·k²)⌋ */
    seq_t t = floor_sqrt((seq_t)5 * kk * kk);

    /* fk = ⌊k · φ⌋ = ⌊(k + t) / 2⌋ */
    seq_t fk = (kk + t) / 2;

    /* a(n) = n + 3 * fk  */
    return (seq_t)n + (seq_t)3 * fk;
}
