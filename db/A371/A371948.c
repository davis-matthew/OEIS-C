/*
 * A371948  Numbers k such that k+1 is composite and A371699(k) != p^2 where p
 *          = A020639(k+1) is the smallest prime factor of k+1.
 * https://oeis.org/A371948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with k+1 composite and A371699(k) != lpf(k+1)^2.
 *            Method: since A371699(k) <= p^2 (p = lpf(k+1)), k is a term iff
 *            some composite m < p^2 divides the base-k concatenation of its
 *            descending prime factors. Odd k and k = 6j+2 give A371699(k) = 4
 *            or 9 = p^2 and are skipped. Terms are foun...
 * Range:     u8 none, u16 n <= 4600, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9998 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *A371948_spf = NULL;
static uint64_t A371948_cap = 0;
static uint64_t *A371948_terms = NULL;
static uint64_t A371948_cnt = 0, A371948_alloc = 0;
static uint64_t A371948_nextk = 2;

static void A371948_sieve(uint64_t limit)
{
    if (limit <= A371948_cap) return;
    uint64_t ncap = A371948_cap ? A371948_cap : 65536;
    while (ncap < limit) ncap *= 2;
    free(A371948_spf);
    A371948_spf = (uint32_t *)calloc((size_t)(ncap + 1), sizeof(uint32_t));
    for (uint64_t i = 2; i <= ncap; ++i) {
        if (A371948_spf[i] == 0) {
            for (uint64_t j = i; j <= ncap; j += i)
                if (A371948_spf[j] == 0) A371948_spf[j] = (uint32_t)i;
        }
    }
    A371948_cap = ncap;
}

static bool A371948_is_term(uint64_t k)
{
    uint64_t N = k + 1;
    uint64_t p = A371948_spf[N];
    if (p == N) return false;            /* prime */
    uint64_t lim = p * p;                /* search m in [4, p^2 - 1] */
    for (uint64_t m = 4; m < lim; ++m) {
        if (A371948_spf[m] == m) continue;   /* prime */
        uint64_t fac[64];
        int c = 0;
        uint64_t t = m;
        while (t > 1) {
            uint64_t q = A371948_spf[t];
            fac[c++] = q;
            t /= q;
        }
        uint64_t rem = 0;
        for (int i = c - 1; i >= 0; --i)
            rem = (rem * k + fac[i]) % m;
        if (rem == 0) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (A371948_cnt < (uint64_t)n) {
        uint64_t k = A371948_nextk++;
        if (k & 1ULL) continue;
        if (k % 6ULL == 2ULL) continue;
        A371948_sieve(k + 2);
        if (A371948_is_term(k)) {
            if (A371948_cnt == A371948_alloc) {
                A371948_alloc = A371948_alloc ? A371948_alloc * 2 : 1024;
                A371948_terms = (uint64_t *)realloc(A371948_terms,
                                    (size_t)A371948_alloc * sizeof(uint64_t));
            }
            A371948_terms[A371948_cnt++] = k;
        }
    }
    return (seq_t)A371948_terms[n - 1];
}
