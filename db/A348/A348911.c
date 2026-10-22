/*
 * A348911  a(n) is the "w" part of f(n) = Sum_{k>=0, d_k>0} w^(d_k-1) *
 *          (-2)^k where Sum_{k>=0} d_k * 4^k is the base-4 representation of
 *          n and w = -1/2 + sqrt(-3)/2 is a primitive cube root of unity;
 *          sequence A348910 gives "real" parts.
 * https://oeis.org/A348911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = the w‑part of Σ_{k≥0, d_k>0} w^(d_k‑1)·(–2)^k, where Σ
 *            d_k·4^k is the base‑4 representation of n and w = –½ + √(–3)/2
 *            is a primitive cube root of unity. The w‑part is computed by
 *            accumulating contributions of each digit: – digit 2 contributes
 *            (+1)·(–2)^k to the w‑part, – digit 3 contribute...
 * Range:     i8 n <= 9562, i16 n <= 9562, i32 n <= 9562, i64 n <= 9562,
 *            i128 n <= 9562, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow = 1;                 /* (-2)^k, starts with k=0 */
    idx_t t = n;                   /* working copy of n */

    while (t != 0) {
        uint32_t digit = (uint32_t)(t & 3u);  /* d_k = n % 4 */
        if (digit == 2u) {
            result += pow;       /* (+1)·(–2)^k */
        } else if (digit == 3u) {
            result -= pow;       /* (–1)·(–2)^k */
        }
        t >>= 2;                 /* n = n / 4 */
        pow *= (seq_t)-2;        /* advance to next power of –2 */
    }

    return result;
}
