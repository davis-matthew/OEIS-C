/*
 * A290127  a(n) = (1/5760)*(n + 5)*(15*n^7 + 225*n^6 + 1265*n^5 + 3707*n^4 +
 *          7120*n^3 + 4900*n^2 - 6480*n + 27648).
 * https://oeis.org/A290127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/5760)*(n + 5)*(15*n^7 + 225*n^6 + 1265*n^5 * +
 *            3707*n^4 + 7120*n^3 + 4900*n^2 * - 6480*n + 27648) (offset 1) *
 *            This implementation uses 128‑bit temporaries to avoid overflow *
 *            for 64‑bit builds; for arbitrary‑precision builds the cast to *
 *            seq_t converts the 128‑bit result into the requ...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 31, u64 n <= 536, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behavior below offset is undefined */

    /* 128‑bit intermediate for safety (works for 64‑bit as well). */
    __int128 tn = (int64_t)n;                   /* convert index        */
    __int128 poly = 15;                         /* a7 */
    poly = poly * tn + 225;                     /* a6 */
    poly = poly * tn + 1265;                    /* a5 */
    poly = poly * tn + 3707;                    /* a4 */
    poly = poly * tn + 7120;                    /* a3 */
    poly = poly * tn + 4900;                    /* a2 */
    poly = poly * tn - 6480;                    /* a1 (negative) */
    poly = poly * tn + 27648;                   /* a0 */

    __int128 result = (tn + 5) * poly / 5760;
    return (seq_t)result;
}
