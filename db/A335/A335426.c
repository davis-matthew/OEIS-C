/*
 * A335426  a(1) = 0; thereafter a(2^(2^k)) = 0 for k > 0, and for other even
 *          numbers n, a(n) = 1+a(n/2), and for odd numbers n, a(n) =
 *          2*a(A064989(n)).
 * https://oeis.org/A335426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(1) = 0; a(2^(2^k)) = 0 for k > 0; other even n: a(n) = 1 +
 *            a(n/2); odd n: a(n) = 2*a(A064989(n))
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 136, u64 n <= 312,
 *            u128 n <= 726, big any n
 * Validated: standard, pass, 98 terms through n = 8160 (2026-11-08)
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

static uint64_t A335426_prev_prime(uint64_t n)
{
    uint64_t p = n - 1;
    for (;; --p)
    {
        if (is_prime((idx_t)p)) return p;
    }
}

/* A064989 on native integers: odd prime p -> prevprime(p), 2 -> 1 */
static uint64_t A335426_A064989(uint64_t n)
{
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);
    uint64_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        if (pr[i] == 2) continue;
        uint64_t q = A335426_prev_prime(pr[i]);
        for (uint64_t k = 0; k < ex[i]; ++k) result *= q;
    }
    return result;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    seq_t mult = 1;
    seq_t add = 0;
    if (m < 1) return 0;
    for (;;)
    {
        if (m == 1) return add;
        if ((m & 1ULL) == 0)
        {
            if ((m & (m - 1)) == 0)
            {
                unsigned e = 0;
                uint64_t t = m;
                while (t > 1) { t >>= 1; ++e; }
                if (e >= 2 && (e & (e - 1)) == 0) return add;
            }
            add += mult;
            m >>= 1;
        }
        else
        {
            mult *= 2;
            m = A335426_A064989(m);
        }
    }
}
