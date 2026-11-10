/*
 * A353870  Primitive terms of A353868.
 * https://oeis.org/A353870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th term k of A353868 (lambda(k) | psi(k)) that is not a
 *            product of smaller terms (>1) and not t*p with t a term, p | t;
 *            a(1)=1. Computed by search
 * Range:     u8 n <= 17, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static uint64_t A353870_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

/* k in A353868: Carmichael lambda(k) divides Dedekind psi(k) */
static int A353870_good(uint64_t k)
{
    if (k == 1) return 1;
    uint64_t n = k, lam = 1, psi = 1;
    for (uint64_t p = 2; p * p <= n; p += (p == 2 ? 1 : 2))
    {
        if (n % p) continue;
        int e = 0;
        uint64_t pe1 = 1;
        while (n % p == 0) { n /= p; if (e) pe1 *= p; ++e; }
        psi *= pe1 * (p + 1);
        uint64_t lp;
        if (p == 2)
        {
            if (e == 1) lp = 1;
            else if (e == 2) lp = 2;
            else lp = pe1 / 2;
        }
        else lp = pe1 * (p - 1);
        lam = (lam / A353870_gcd(lam, lp)) * lp;
    }
    if (n > 1)
    {
        psi *= (n + 1);
        uint64_t lp = n - 1;
        lam = (lam / A353870_gcd(lam, lp)) * lp;
    }
    return psi % lam == 0;
}

/* m is a product of terms >1 of A353868 (m=1 true) */
static int A353870_reach(uint64_t m)
{
    if (m == 1) return 1;
    for (uint64_t d = 2; d <= m; d++)
        if (m % d == 0 && A353870_good(d) && A353870_reach(m / d)) return 1;
    return 0;
}

static int A353870_primitive(uint64_t k)
{
    if (!A353870_good(k)) return 0;
    if (k == 1) return 1;
    for (uint64_t d = 2; d < k; d++)
    {
        if (k % d) continue;
        if (A353870_good(d) && A353870_reach(k / d)) return 0;
    }
    for (uint64_t p = 2; p * p <= k; p++)
    {
        if (k % (p * p)) continue;
        /* p^2 | k; need p prime */
        int isp = 1;
        for (uint64_t q = 2; q * q <= p; q++) if (p % q == 0) { isp = 0; break; }
        if (isp && A353870_good(k / p)) return 0;
    }
    return 1;
}

static seq_t *A353870_arr = NULL;
static size_t A353870_cap = 0, A353870_len = 0;
static uint64_t A353870_next = 1;

seq_t a(idx_t n)
{
    while (A353870_len < (size_t)n)
    {
        if (A353870_primitive(A353870_next))
        {
            if (A353870_len + 1 > A353870_cap)
            {
                size_t nc = A353870_cap ? A353870_cap * 2 : 16;
                A353870_arr = mem_resize(seq_t, A353870_arr, nc);
                A353870_cap = nc;
            }
            A353870_arr[A353870_len++] = (seq_t)A353870_next;
        }
        ++A353870_next;
    }
    return A353870_arr[(size_t)n - 1];
}
