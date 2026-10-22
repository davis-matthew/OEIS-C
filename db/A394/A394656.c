/*
 * A394656  a(n) = binomial(n,3) + 4*binomial(n,4) + 10*binomial(n,5) +
 *          20*binomial(n,6).
 * https://oeis.org/A394656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,3) + 4*binomial(n,4) + 10*binomial(n,5) +
 *            20*binomial(n,6)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    if (n < 3) return 0;
    /* use 128‑bit arithmetic for intermediates to avoid overflow for
       64‑bit seq_t builds; cast back to seq_t at the end. */
    __int128 t = 0;
    /* binomial(n,3) = n*(n-1)*(n-2)/6 */
    t += (__int128)n * (n-1) * (n-2) / 6;
    /* binomial(n,4) = n*(n-1)*(n-2)*(n-3)/24, coefficient 4 */
    t += 4 * (__int128)n * (n-1) * (n-2) * (n-3) / 24;
    /* binomial(n,5) = n*(n-1)*(n-2)*(n-3)*(n-4)/120, coefficient 10 */
    t += 10 * (__int128)n * (n-1) * (n-2) * (n-3) * (n-4) / 120;
    /* binomial(n,6) = n*(n-1)*(n-2)*(n-3)*(n-4)*(n-5)/720, coefficient 20 */
    t += 20 * (__int128)n * (n-1) * (n-2) * (n-3) * (n-4) * (n-5) / 720;
    return (seq_t)t;
}
