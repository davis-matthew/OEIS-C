/*
 * A386390  Numbers k such that k-1 | sigma+(k) where sigma+ is A107758.
 * https://oeis.org/A386390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, changed
 * Formula:   a search, not a closed form. It finds all k <= 2^20 with * (k-1)
 *            | A107758(k) using a smallest-prime-factor sieve. It then closes
 *            the * set under k -> k*(2k-1) whenever 2k-1 is prime. If
 *            sigma+(k) = m*(k-1) and * p = 2k-1 is prime, then sigma+(k*p) =
 *            m*(k-1)*(2k+1) = m*(k*p-1), so k*p is * also a...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

typedef unsigned __int128 A386390_u128;

static A386390_u128 *A386390_terms = NULL;
static size_t A386390_cnt = 0;
static size_t A386390_cap = 0;
static bool A386390_built = false;

static void A386390_push(A386390_u128 v)
{
    for (size_t i = 0; i < A386390_cnt; ++i)
        if (A386390_terms[i] == v) return;
    if (A386390_cnt == A386390_cap) {
        size_t nc = A386390_cap ? A386390_cap * 2 : 64;
        A386390_u128 *t = (A386390_u128 *)realloc(A386390_terms, nc * sizeof(A386390_u128));
        if (!t) return;
        A386390_terms = t;
        A386390_cap = nc;
    }
    A386390_terms[A386390_cnt++] = v;
}

static int A386390_cmp(const void *x, const void *y)
{
    A386390_u128 a = *(const A386390_u128 *)x, b = *(const A386390_u128 *)y;
    return (a > b) - (a < b);
}

static void A386390_build(void)
{
    const uint32_t B = 1u << 20;
    uint32_t *spf = (uint32_t *)calloc((size_t)B + 1, sizeof(uint32_t));
    if (!spf) return;
    for (uint32_t i = 2; i <= B; ++i) {
        if (spf[i] == 0) {
            for (uint64_t j = i; j <= B; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }
    /* brute-force seeds: sigma+(k) = Product (1 + (p^(e+1)-1)/(p-1)) */
    for (uint32_t k = 2; k <= B; ++k) {
        uint64_t m = k;
        A386390_u128 s = 1;
        while (m > 1) {
            uint64_t p = spf[m];
            uint64_t pw = 1, sum = 0;
            while (m % p == 0) { m /= p; pw *= p; sum += pw; }
            s *= (A386390_u128)(2 + sum);
        }
        if (s % (A386390_u128)(k - 1) == 0) A386390_push((A386390_u128)k);
    }
    free(spf);

    /* closure under k -> k*(2k-1) when 2k-1 is prime */
    for (size_t i = 0; i < A386390_cnt; ++i) {
        A386390_u128 t = A386390_terms[i];
        if (t >= ((A386390_u128)1 << 62)) continue;
        uint64_t q = (uint64_t)(2 * t - 1);
        if (is_prime_u64(q)) A386390_push(t * (A386390_u128)q);
    }
    qsort(A386390_terms, A386390_cnt, sizeof(A386390_u128), A386390_cmp);
    A386390_built = true;
}

seq_t a(idx_t n)
{
    if (!A386390_built) A386390_build();
    if (n < 1 || n > (idx_t)A386390_cnt) return (seq_t)0;
    A386390_u128 v = A386390_terms[n - 1];
    uint64_t hi = (uint64_t)(v >> 64);
    uint64_t lo = (uint64_t)v;
    seq_t r = (seq_t)lo;
    if (hi != 0) {
        seq_t h = (seq_t)hi;
        h <<= 32;
        h <<= 32;
        r += h;
    }
    return r;
}
