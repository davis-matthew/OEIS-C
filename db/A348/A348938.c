/*
 * A348938  Odd numbers k for which A064989(sigma(k)) < A064989(k), and which
 *          are of the form p^(1+4k) * r^2, where p is prime of the form 1+4m,
 *          r > 1, and gcd(p,r) = 1.
 * https://oeis.org/A348938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th odd k = p^e * r^2 (p prime == 1 mod 4, e == 1 mod 4,
 *            r > 1, gcd(p,r) = 1) with A064989(sigma(k)) < A064989(k); found
 *            by a sieve-free search with memoization
 * Range:     u8 n <= 2, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-10)
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

static uint64_t A064989_val(uint64_t m)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    uint64_t res = 1;
    factor_u64(m, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i)
    {
        if (pr[i] == 2) continue;
        uint64_t q = pr[i] - 1;
        while (!is_prime((idx_t)q)) --q;
        for (uint64_t j = 0; j < ex[i]; ++j) res *= q;
    }
    return res;
}

static bool A348938_check(uint64_t m)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(m, pr, ex, &cnt);
    int odd_cnt = 0;
    uint64_t sig = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = pr[i], e = ex[i];
        if (e & 1)
        {
            ++odd_cnt;
            if (odd_cnt > 1) return false;
            if ((p & 3) != 1 || (e & 3) != 1) return false;
        }
        uint64_t s = 1, t = 1;
        for (uint64_t j = 0; j < e; ++j)
        {
            t *= p;
            s += t;
        }
        sig *= s;
    }
    if (odd_cnt != 1) return false;
    if (cnt == 1) return false; /* r > 1 required */
    return A064989_val(sig) < A064989_val(m);
}

static uint64_t *A348938_terms = NULL;
static size_t A348938_cnt = 0, A348938_cap = 0;
static uint64_t A348938_next = 1;

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (A348938_cnt < (size_t)n)
    {
        uint64_t m = A348938_next;
        A348938_next += 2;
        if (m > 1 && A348938_check(m))
        {
            if (A348938_cnt == A348938_cap)
            {
                size_t nc = A348938_cap ? A348938_cap * 2 : 256;
                A348938_terms = (uint64_t *)realloc(A348938_terms, nc * sizeof(uint64_t));
                A348938_cap = nc;
            }
            A348938_terms[A348938_cnt++] = m;
        }
    }
    return (seq_t)A348938_terms[n - 1];
}
