/*
 * A337343  Numbers k such that A064989(sigma(k)) >= k.
 * https://oeis.org/A337343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k (searching k = 1, 2, 3, ...) such that
 *            A064989(sigma(k)) >= k
 * Range:     u8 n <= 13, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-08)
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

static uint64_t A337343_prev_prime(uint64_t n)
{
    uint64_t p = n - 1;
    for (;; --p)
    {
        if (is_prime((idx_t)p)) return p;
    }
}

static uint64_t A337343_sigma(uint64_t k)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0, i;
    uint64_t s = 1;
    factor_u64(k, pr, ex, &cnt);
    for (i = 0; i < cnt; ++i)
    {
        uint64_t term = 1, pw = 1, e;
        for (e = 0; e < ex[i]; ++e)
        {
            pw *= pr[i];
            term += pw;
        }
        s *= term;
    }
    return s;
}

static uint64_t A337343_A064989(uint64_t m)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0, i;
    uint64_t res = 1;
    if (m < 1) return 0;
    factor_u64(m, pr, ex, &cnt);
    for (i = 0; i < cnt; ++i)
    {
        uint64_t base, e;
        if (pr[i] == 2) continue;
        base = A337343_prev_prime(pr[i]);
        for (e = 0; e < ex[i]; ++e) res *= base;
    }
    return res;
}

seq_t a(idx_t n)
{
    static idx_t last_n = 0;
    static uint64_t last_k = 0;
    idx_t cnt;
    uint64_t k;
    if (n < 1) return 0;
    if (last_n != 0 && last_n <= n)
    {
        cnt = last_n;
        k = last_k;
    }
    else
    {
        cnt = 0;
        k = 0;
    }
    while (cnt < n)
    {
        ++k;
        if (A337343_A064989(A337343_sigma(k)) >= k) ++cnt;
    }
    last_n = cnt;
    last_k = k;
    return (seq_t)k;
}
