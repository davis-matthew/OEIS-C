/*
 * A303590  Floor(n*beta)-1, where 1/alpha+1/beta=1, alpha being the number
 *          with continued fraction expansion [1;1,2,3,4,5,...] (A247844).
 * https://oeis.org/A303590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊n·β⌋ – 1, where β = α/(α–1) and α = [1;1,2,3,4,5,…].
 *            Using the 10‑th convergent of α = 12714721/7489051 gives β =
 *            12714721/5225670
 * Range:     u8 none, u16 none, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t P = 12714721;   /* numerator of β */
static const seq_t D = 5225670;   /* denominator of β */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n * P / D;   /* floor(n·β) */
    return t - 1;
}
