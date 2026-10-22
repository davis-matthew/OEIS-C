/*
 * A396714  Maximum number of regions that can be formed in the plane by
 *          drawing n lollipop (or qoppa) shapes of equal radii.
 * https://oeis.org/A396714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2, a(2)=10. For n≥3: let d1(1)=1 and d2(k)=7 for
 *            k≡0,1 (mod 3), d2(k)=4 for k≡2 (mod 3). Then
 *            d1(k)=d1(k‑1)+d2(k‑1) and a(n)=a(n‑1)+d1(n‑1)
 * Range:     u8 n <= 9, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    seq_t a0 = (seq_t)1;          /* a(0) */
    seq_t a1 = (seq_t)2;          /* a(1) */
    seq_t d1 = (seq_t)1;          /* difference a(1) – a(0) */

    for (idx_t i = 2; i <= n; ++i) {
        /* Second difference (pattern 7,7,4 repeating) */
        seq_t d2 = ((i - 2) % 3 == 2) ? (seq_t)4 : (seq_t)7;
        d1 += d2;                 /* update first difference */
        seq_t ai = a1 + d1;       /* compute a(i) */
        a0 = a1;
        a1 = ai;
    }

    return a1;
}
