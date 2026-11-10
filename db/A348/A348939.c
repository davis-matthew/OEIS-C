/*
 * A348939  Odd numbers k for which A064989(sigma(k)) > A064989(k), and which
 *          are of the form p^(1+4k) * r^2, where p is prime of the form 1+4m,
 *          r > 1, and gcd(p,r) = 1.
 * https://oeis.org/A348939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th odd k = p^(1+4j)*r^2 (p prime == 1 mod 4, r>1,
 *            gcd(p,r)=1) with A064989(sigma(k)) > A064989(k); found by a scan
 *            over odd k
 * Range:     u8 n <= 2, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-10)
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

static uint64_t A348939_prev_prime(uint64_t n)
{
    uint64_t p = n - 1;
    for (;; --p)
    {
        if (is_prime((idx_t)p)) return p;
    }
}

static uint64_t A348939_a064989(uint64_t m)
{
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    uint64_t res = 1;
    factor_u64(m, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i)
    {
        if (pr[i] == 2) continue;
        uint64_t b = A348939_prev_prime(pr[i]);
        for (uint64_t e = 0; e < ex[i]; ++e) res *= b;
    }
    return res;
}

static bool A348939_test(uint64_t k)
{
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(k, pr, ex, &cnt);
    if (cnt < 2) return false;
    int odd_count = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        if (ex[i] & 1ULL)
        {
            odd_count++;
            if (odd_count > 1) return false;
            if (pr[i] % 4 != 1) return false;
            if (ex[i] % 4 != 1) return false;
        }
    }
    if (odd_count != 1) return false;

    uint64_t sg = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t s = 1, t = 1;
        for (uint64_t e = 0; e < ex[i]; ++e)
        {
            t *= pr[i];
            s += t;
        }
        sg *= s;
    }
    return A348939_a064989(sg) > A348939_a064989(k);
}

static uint64_t *A348939_terms = NULL;
static size_t A348939_count = 0;
static size_t A348939_cap = 0;
static uint64_t A348939_next = 1;

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (A348939_count < (size_t)n)
    {
        uint64_t k = A348939_next;
        A348939_next += 2;
        if (A348939_test(k))
        {
            if (A348939_count == A348939_cap)
            {
                size_t nc = A348939_cap ? A348939_cap * 2 : 256;
                uint64_t *np = (uint64_t *)realloc(A348939_terms, nc * sizeof(uint64_t));
                if (!np) return 0;
                A348939_terms = np;
                A348939_cap = nc;
            }
            A348939_terms[A348939_count++] = k;
        }
    }
    return (seq_t)A348939_terms[n - 1];
}
