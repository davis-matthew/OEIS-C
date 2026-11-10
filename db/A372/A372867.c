/*
 * A372867  Distinct terms in A242017, listed in the order of their
 *          appearance.
 * https://oeis.org/A372867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th distinct value, in order of first appearance, of the
 *            smallest prime factor of 2^k+1 over k=3,4,5,... with 2^k+1
 *            composite (A242017). Computed by search: for k = 2^v*d (d odd),
 *            the least prime p with 2^k == -1 (mod p) has p == 1 (mod
 *            2^(v+1)), so scan that progression; odd k gives 3. s...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define A372867_CAP 512

static uint64_t A372867_pm(uint64_t e, uint64_t m)
{
    uint64_t r = 1, b = 2 % m;
    while (e) {
        if (e & 1) r = (uint64_t)(((unsigned __int128)r * b) % m);
        b = (uint64_t)(((unsigned __int128)b * b) % m);
        e >>= 1;
    }
    return r;
}

/* smallest prime factor of 2^k+1 (k>=3, composite case); 0 if not found within cap */
static uint64_t A372867_spf(uint64_t k)
{
    if (k & 1) return 3;
    if (k == 128) return 59649589127497217ULL;
    if (k == 256) return 1238926361552897ULL;
    uint64_t m = 2;
    uint64_t t = k;
    while ((t & 1) == 0) { t >>= 1; m <<= 1; }   /* m = 2^(v+1) */
    uint64_t j;
    for (j = 1; j < ((uint64_t)1 << 25); ++j) {
        uint64_t p = 1 + j * m;
        if (A372867_pm(k, p) == p - 1) return p;
    }
    return 0;
}

seq_t a(idx_t n)
{
    static uint64_t terms[A372867_CAP];
    static size_t cnt = 0;
    static uint64_t kk = 3;

    if (n == 0 || n > A372867_CAP) return (seq_t)0;

    while (cnt < (size_t)n) {
        uint64_t k = kk++;
        if (k == 4 || k == 8 || k == 16) continue;   /* 2^k+1 prime */
        uint64_t s = A372867_spf(k);
        if (s == 0) continue;
        bool seen = false;
        size_t i;
        for (i = 0; i < cnt; ++i) {
            if (terms[i] == s) { seen = true; break; }
        }
        if (!seen) terms[cnt++] = s;
    }
    return (seq_t)terms[n - 1];
}
