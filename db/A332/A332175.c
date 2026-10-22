/*
 * A332175  a(n) = 7*(10^(2n+1)-1)/9 - 2*10^n.
 * https://oeis.org/A332175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 7*(10^(2n+1)-1)/9 - 2*10^n, offset 0
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* a(0) = 5 */
    if (n == 0)
        return (seq_t)5;

    /* For built‑in integer seq_t types (≤ 64 bits) use 128‑bit intermediates */
    if (sizeof(seq_t) <= 8) {
        idx_t e1 = 2 * n + 1;
        idx_t e2 = n;

        unsigned __int128 pow1 = 1;
        for (idx_t i = 0; i < e1; ++i)
            pow1 *= 10;

        unsigned __int128 pow2 = 1;
        for (idx_t i = 0; i < e2; ++i)
            pow2 *= 10;

        unsigned __int128 tmp  = pow1 - 1;
        unsigned __int128 m    = ((unsigned __int128)7 * tmp) / 9;
        unsigned __int128 res  = m - ((unsigned __int128)2 * pow2);

        return (seq_t)res;
    }

    /* For arbitrary‑precision seq_t types use the provided pow10() */
    seq_t pow1 = pow10((idx_t)(2 * n + 1));
    seq_t pow2 = pow10(n);
    seq_t tmp  = pow1 - (seq_t)1;
    seq_t m    = ((seq_t)7 * tmp) / (seq_t)9;
    seq_t res  = m - (((seq_t)2 * pow2));

    return res;
}
