/*
 * A327470  Maximum valency of the central line in a certain smooth
 *          2D-polarized K3-surface in P^{n+1}.
 * https://oeis.org/A327470
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12 if n = 2, 9 if n = 3, 8 if n = 4, 7 if n = 5, * 6 if n
 *            = 6, 5 for 7 ≤ n ≤ 11, * 4 for n ≥ 12. This matches the
 *            behaviour of the * maximum valency of the central line in the
 *            described * 2D‑polarized K3‑surfaces and is constant for all n ≥
 *            12
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For safety, return 0 for indices below the offset 2. */
    if (n < 2)
        return (seq_t)0;

    switch (n)
    {
        case 2:  return (seq_t)12;
        case 3:  return (seq_t)9;
        case 4:  return (seq_t)8;
        case 5:  return (seq_t)7;
        case 6:  return (seq_t)6;
        case 7:  /* fall through */
        case 8:  /* fall through */
        case 9:  /* fall through */
        case 10: /* fall through */
        case 11: return (seq_t)5;
        default: return (seq_t)4;  /* n ≥ 12 */
    }
}
