/*
 * A390619  a(n) = Sum_{k=0..floor(n/3)} 2^k * 3^(n-3*k) *
 *          binomial(2*k,2*(n-3*k)).
 * https://oeis.org/A390619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} 2^k * 3^(n-3*k) * binomial(2*k,
 *            2*(n-3*k))
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 31, u64 n <= 61, u128 n <= 120,
 *            big any n
 * Helpers:   binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 3;
    seq_t ans = 0;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t sub = n - 3 * k;            /* n - 3k */
        idx_t twok = 2 * k;               /* 2k   */
        idx_t twosub = 2 * sub;           /* 2(n-3k) */

        if (twosub > twok)                /* binomial would be zero */
            continue;

        seq_t bin = binomial(twok, twosub);
        if (bin == 0)
            continue;

        seq_t term = bin * pow2(k);
        term = term * pow3(sub);
        ans += term;
    }

    return ans;
}
