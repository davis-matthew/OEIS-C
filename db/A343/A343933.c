/*
 * A343933  a(n) = (Sum_{k=1..n} (-k)^k) mod n.
 * https://oeis.org/A343933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (Sum_{k=1..n} (-k)^k) mod n
 * Range:     u8 n <= 277, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety – a(0) is undefined, return 0 */
    idx_t sum_mod = 0;

    for (idx_t k = 1; k <= n; ++k) {
        /* compute k^k mod n, then apply the sign (-1)^k */
        seq_t t = mod_pow((seq_t)k, k, (seq_t)n);
        idx_t t_mod = (idx_t)t;

        if (k & 1) {                    /* odd k → negative term */
            if (t_mod != 0)
                sum_mod += n - t_mod;
        } else {                        /* even k → positive term */
            sum_mod += t_mod;
        }
        sum_mod %= n;                   /* keep the sum within [0, n-1] */
    }

    return (seq_t)sum_mod;
}
