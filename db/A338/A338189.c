/*
 * A338189  Integers m such that A337454(m) = 1.
 * https://oeis.org/A338189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th integer m such that A337454(m) = 1 (A337454(m)
 *            counts divisors d of m for which #{x<m: x^d==x}/#{x<m: -x^d==x}
 *            divides m; counts are multiplicative via CRT, found by search)
 * Range:     u8 n <= 6, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-08)
 * Helpers:   powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/powmod_u64.h"

/* does divisor d of k satisfy the A337454 condition; prime-power factors given */
static int A338189_ok(uint64_t d, const uint64_t *q, int cnt, uint64_t k)
{
    uint64_t c1 = 1, c2 = 1;
    for (int i = 0; i < cnt; ++i)
    {
        uint64_t Q = q[i], a = 0, b = 0;
        for (uint64_t m = 0; m < Q; ++m)
        {
            uint64_t p = powmod_u64(m, d, Q);
            if (p == m) ++a;
            if ((p + m) % Q == 0) ++b;
        }
        c1 *= a;
        c2 *= b;
    }
    if (c2 == 0) return 0;
    if (c1 % c2 != 0) return 0;
    return (k % (c1 / c2)) == 0;
}

/* true iff A337454(k) == 1 */
static int A338189_is_one(uint64_t k)
{
    uint64_t q[20];
    int cnt = 0;
    uint64_t t = k;
    for (uint64_t p = 2; p * p <= t; ++p)
    {
        if (t % p == 0)
        {
            uint64_t Q = 1;
            while (t % p == 0) { t /= p; Q *= p; }
            q[cnt++] = Q;
        }
    }
    if (t > 1) q[cnt++] = t;

    for (uint64_t i = 1; i * i <= k; ++i)
    {
        if (k % i != 0) continue;
        uint64_t j = k / i;
        if (i != 1 && A338189_ok(i, q, cnt, k)) return 0;
        if (j != i && j != 1 && A338189_ok(j, q, cnt, k)) return 0;
    }
    return 1;
}

static uint64_t *A338189_terms = 0;
static size_t A338189_count = 0, A338189_cap = 0;
static uint64_t A338189_next = 1;

seq_t a(idx_t n)
{
    size_t want = (size_t)n;
    while (A338189_count < want)
    {
        if (A338189_is_one(A338189_next))
        {
            if (A338189_count == A338189_cap)
            {
                A338189_cap = A338189_cap ? A338189_cap * 2 : 64;
                A338189_terms = (uint64_t *)realloc(A338189_terms, A338189_cap * sizeof(uint64_t));
            }
            A338189_terms[A338189_count++] = A338189_next;
        }
        ++A338189_next;
    }
    return (seq_t)A338189_terms[want - 1];
}
