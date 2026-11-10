/*
 * A379114  Numbers k for which A379113(k) > 1, i.e., k that have a proper
 *          unitary divisor d > 1 such that
 *          A048720(A065621(sigma(d)),sigma(k/d)) is equal to sigma(k).
 * https://oeis.org/A379114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that some unitary divisor d (1<d<k) satisfies
 *            A048720(A065621(sigma(d)), sigma(k/d)) = sigma(k); computed by a
 *            search over k with memoized terms
 * Range:     u8 n <= 136, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 91 terms through n = 9303 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned __int128 a379114_u128;

static a379114_u128 a379114_rev(a379114_u128 x)
{
    a379114_u128 low = x & (~x + 1);
    return x ^ ((x - low) << 1);
}

static a379114_u128 a379114_gf2mul(a379114_u128 i, a379114_u128 j)
{
    a379114_u128 r = 0;
    while (j) {
        if (j & 1) r ^= i;
        i <<= 1;
        j >>= 1;
    }
    return r;
}

static bool a379114_test(uint64_t k)
{
    uint64_t sg[20];
    uint64_t pw[20];
    int cnt = 0;
    uint64_t m = k;
    uint64_t p;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = 1, s = 1, t = 1;
            while (m % p == 0) {
                m /= p;
                t *= p;
                s += t;
                q = t;
            }
            pw[cnt] = q;
            sg[cnt] = s;
            cnt++;
        }
    }
    if (m > 1) {
        pw[cnt] = m;
        sg[cnt] = m + 1;
        cnt++;
    }
    if (cnt < 2) return false;
    uint64_t sk = 1;
    int i;
    for (i = 0; i < cnt; i++) sk *= sg[i];
    uint32_t mask;
    uint32_t full = ((uint32_t)1 << cnt) - 1;
    for (mask = 1; mask < full; mask++) {
        uint64_t sd = 1;
        for (i = 0; i < cnt; i++)
            if (mask & ((uint32_t)1 << i)) sd *= sg[i];
        uint64_t so = sk / sd;
        a379114_u128 v = a379114_gf2mul(a379114_rev(sd), so);
        if (v == (a379114_u128)sk) return true;
    }
    return false;
}

static uint64_t *a379114_terms = 0;
static size_t a379114_count = 0;
static size_t a379114_cap = 0;
static uint64_t a379114_next = 2;

seq_t a(idx_t n)
{
    while (a379114_count < (size_t)n) {
        uint64_t k = a379114_next++;
        if (a379114_test(k)) {
            if (a379114_count == a379114_cap) {
                a379114_cap = a379114_cap ? a379114_cap * 2 : 1024;
                a379114_terms = (uint64_t *)realloc(a379114_terms, a379114_cap * sizeof(uint64_t));
            }
            a379114_terms[a379114_count++] = k;
        }
    }
    return (seq_t)a379114_terms[(size_t)n - 1];
}
