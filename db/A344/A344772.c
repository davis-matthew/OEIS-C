/*
 * A344772  Ordinal transform of infinitary phi, A091732.
 * https://oeis.org/A344772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = #{k : 1 <= k <= n, A091732(k) = A091732(n)}, where
 *            A091732(n) = Product_{p^e||n} Product_{bits 2^j of e} (p^(2^j) -
 *            1)
 * Range:     u8 n <= 312, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9234 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A344772_iphi(idx_t n)
{
    idx_t m = n;
    seq_t res = 1;
    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p != 0)
            continue;
        idx_t e = 0;
        while (m % p == 0)
        {
            m /= p;
            ++e;
        }
        seq_t q = (seq_t)p;
        while (e)
        {
            if (e & 1)
                res *= (q - 1);
            e >>= 1;
            if (e)
                q *= q;
        }
    }
    if (m > 1)
        res *= ((seq_t)m - 1);
    return res;
}

seq_t a(idx_t n)
{
    seq_t target = A344772_iphi(n);
    idx_t cnt = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        if (A344772_iphi(k) == target)
            ++cnt;
    }
    return (seq_t)cnt;
}
