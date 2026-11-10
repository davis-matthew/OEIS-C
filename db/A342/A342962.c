/*
 * A342962  a(n) is the least prime that starts a string of exactly n distinct
 *          primes p_1, p_2, ..., p_n where p_{i+1} = p_i+A085563(p_i), but
 *          p_n+A085563(p_n) is either not prime or equal to p_n.
 * https://oeis.org/A342962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = least prime p_1 whose chain p_{i+1} = p_i + A085563(p_i)
 *            has exactly n distinct primes; found by a doubling-limit sieve
 *            search
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t A342962_ps(uint64_t x)
{
    uint64_t s = 0;
    while (x > 0)
    {
        unsigned d = (unsigned)(x % 10);
        if (d == 2 || d == 3 || d == 5 || d == 7)
            s += d;
        x /= 10;
    }
    return s;
}

static int A342962_isp(const uint8_t *comp, uint64_t M, uint64_t x)
{
    if (x > M || x < 2)
        return 0;
    if (x == 2)
        return 1;
    if ((x & 1) == 0)
        return 0;
    return comp[x >> 1] == 0;
}

seq_t a(idx_t n)
{
    uint64_t N = 1u << 12;
    for (;;)
    {
        uint64_t M = N + 1000;
        size_t sz = (size_t)(M / 2 + 1);
        uint8_t *comp = (uint8_t *)calloc(sz, 1);
        if (!comp)
            return 0;
        for (uint64_t i = 3; i * i <= M; i += 2)
        {
            if (comp[i >> 1])
                continue;
            for (uint64_t j = i * i; j <= M; j += 2 * i)
                comp[j >> 1] = 1;
        }
        for (uint64_t p = 2; p <= N; p = (p == 2) ? 3 : p + 2)
        {
            if (!A342962_isp(comp, M, p))
                continue;
            uint64_t cur = p;
            uint64_t cnt = 1;
            for (;;)
            {
                uint64_t q = cur + A342962_ps(cur);
                if (q == cur || !A342962_isp(comp, M, q))
                    break;
                cur = q;
                cnt++;
                if (cnt > (uint64_t)n)
                    break;
            }
            if (cnt == (uint64_t)n)
            {
                free(comp);
                return (seq_t)p;
            }
        }
        free(comp);
        N *= 2;
    }
}
