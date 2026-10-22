/*
 * A330185  a(n) = n + floor(n*s/r) + floor(n*t/r), where r = phi - 1/2, s =
 *          phi, t = phi + 1/2, and phi is the golden ratio = (1+sqrt(5))/2.
 * https://oeis.org/A330185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + ⌊n*φ/r⌋ + ⌊n*t/r⌋ where r = φ-½, φ = (1+√5)/2, t =
 *            φ+½. This simplifies to a(n) = 3n + ⌊n/√5⌋ + ⌊2n/√5⌋. We compute
 *            ⌊n/√5⌋ and ⌊2n/√5⌋ exactly by integer arithmetic: ⌊n/√5⌋ = ⌊√(
 *            ⌊n²/5⌋ )⌋ ⌊2n/√5⌋ = ⌊√( ⌊4n²/5⌋ )⌋ The integer square root is
 *            obtained with binary search
 * Range:     u8 n <= 8, u16 n <= 35, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t isqrt_seq(seq_t x)
{
    if (x == (seq_t)0) return (seq_t)0;
    seq_t low = (seq_t)0;
    seq_t high = x;
    seq_t ans = (seq_t)0;
    while (low <= high)
    {
        seq_t mid = (low + high) / (seq_t)2;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
        {
            ans = mid;
            low = mid + (seq_t)1;
        }
        else
        {
            if (mid == (seq_t)0) break;
            high = mid - (seq_t)1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t n_sq = sn * sn;          /* n²          */
    seq_t m1 = n_sq / (seq_t)5;    /* ⌊n²/5⌋      */
    seq_t f1 = isqrt_seq(m1);      /* ⌊√⌊n²/5⌋⌋   */

    seq_t m2 = (n_sq * (seq_t)4) / (seq_t)5;  /* ⌊4n²/5⌋  */
    seq_t f2 = isqrt_seq(m2);      /* ⌊√⌊4n²/5⌋⌋ */

    return (sn + sn + sn) + f1 + f2; /* 3n + f1 + f2 */
}
