/*
 * A340078  a(n) = gcd(n, 1+A018804(n)), where A018804(n) = Sum_{k=1..n}
 *          gcd(k, n).
 * https://oeis.org/A340078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, 1 + sum_{k=1..n} gcd(k,n))
 * Range:     u8 n <= 59, u16 n <= 7685, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    const uint64_t nn = (uint64_t)n;

    /* factor n */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(nn, pr, ex, &cnt);

    /* enumerate all divisors of n */
    uint64_t divs[256];
    size_t len = 1;
    divs[0] = 1;
    for (size_t i = 0; i < cnt; ++i) {
        const uint64_t p = pr[i];
        const uint64_t e = ex[i];
        const size_t old_len = len;
        uint64_t mul = 1;
        for (uint64_t j = 1; j <= e; ++j) {
            mul *= p;
            for (size_t k = 0; k < old_len; ++k)
                divs[len++] = divs[k] * mul;
        }
    }

    /* compute the sum of gcd(k,n) via divisors */
    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i) {
        const uint64_t d = divs[i];
        uint64_t m   = nn / d;
        uint64_t phi = m;
        for (size_t j = 0; j < cnt; ++j) {
            const uint64_t p = pr[j];
            if (m % p == 0) {
                while (m % p == 0) m /= p;
                phi = phi / p * (p - 1);
            }
        }
        sum += (seq_t)d * (seq_t)phi;
    }

    return gcd((seq_t)n, sum + 1);
}
