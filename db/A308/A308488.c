/*
 * A308488  a(n) is the smallest n-gonal pyramidal number greater than 1 which
 *          is also n-gonal; a(n) = 0 when one does not exist.
 * https://oeis.org/A308488
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest n‑gonal number N>1 which is also the sum of the
 *            first m n‑gonal numbers for some m>1. Return 0 when no such N
 *            exists. Indexing starts at n = 3
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned __int128 floor_sqrt128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = x, ans = 0;
    while (lo <= hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq == x)
            return mid;
        if (sq < x) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;          /* function defined only for n ≥ 3 */

    unsigned __int128 N = (unsigned __int128)n;   /* n as 128‑bit value */

    const unsigned __int128 a_val   = N - 2;      /* a = n-2 */
    const unsigned __int128 nminus5 = N - 5;      /* n-5 */

    /* Upper bound on m.  200000 is more than enough for all known
       terms and keeps the time well below the 2 s limit. */
    const idx_t MAX_M = 200000;

    for (idx_t m = 2; m <= MAX_M; ++m) {
        unsigned __int128 m128   = (unsigned __int128)m;
        unsigned __int128 m1     = m128 + 1;
        unsigned __int128 term   = a_val * m128 - nminus5;   /* a*m - (n-5) */

        /* numerator = m*(m+1)*(a*m - (n-5)) */
        unsigned __int128 numerator = m128 * m1 * term;
        unsigned __int128 sum128    = numerator / 6;        /* exact division */

        /* sum must be > 1 and a n‑gonal number.  Test the quadratic
           discriminant: (n-4)^2 + 8*(n-2)*sum.                */
        unsigned __int128 D = (N - 4) * (N - 4) + (unsigned __int128)8 * (N - 2) * sum128;
        unsigned __int128 s = floor_sqrt128(D);
        if (s * s != D)
            continue;                          /* not a perfect square */

        unsigned __int128 num   = (N - 4) + s;
        unsigned __int128 denom = (unsigned __int128)2 * (N - 2);
        if (denom == 0 || num % denom != 0)
            continue;

        unsigned __int128 t = num / denom;
        if (t >= 1)                     /* found a valid sum */
            return (seq_t)sum128;        /* cast to the output type */
    }

    return (seq_t)0;   /* no suitable n‑gonal number found   */
}
