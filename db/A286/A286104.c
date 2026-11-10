/*
 * A286104  If A286103(A285734(n)) < A286103(A285735(n)), a(n) = A285734(n),
 *          otherwise a(n) = A285735(n), a(1) = 0.
 * https://oeis.org/A286104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; for n>1, a(n) = A285734(n) if A286103(A285734(n)) <
 *            A286103(A285735(n)), else A285735(n); computed by direct search
 *            with memoised A286103
 * Range:     u8 n <= 510, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 112 terms through n = 9385 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool A286104_sqf(uint64_t x)
{
    if (x <= 1) return true;
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            x /= p;
            if (x % p == 0) return false;
        }
    }
    return true;
}

/* largest squarefree L <= n/2 with n-L squarefree */
static idx_t A286104_low(idx_t n)
{
    idx_t L;
    for (L = n / 2; L >= 1; --L)
        if (A286104_sqf(L) && A286104_sqf(n - L))
            return L;
    return 1;
}

static unsigned char *A286104_cache = NULL;
static size_t A286104_cap = 0;

/* A286103: depth value, a(1)=0, a(n)=1+min(a(L),a(n-L)) */
static unsigned A286104_d(idx_t n)
{
    if (n <= 1) return 0;
    if (n >= A286104_cap) {
        size_t nc = A286104_cap ? A286104_cap : 64;
        while (nc <= n) nc *= 2;
        unsigned char *t = (unsigned char *)realloc(A286104_cache, nc);
        if (!t) abort();
        for (size_t i = A286104_cap; i < nc; ++i) t[i] = 0;
        A286104_cache = t;
        A286104_cap = nc;
    }
    if (A286104_cache[n]) return A286104_cache[n];
    idx_t L = A286104_low(n);
    unsigned l = A286104_d(L);
    unsigned r = A286104_d(n - L);
    unsigned v = 1 + (l < r ? l : r);
    A286104_cache[n] = (unsigned char)v;
    return v;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    idx_t L = A286104_low(n);
    idx_t R = n - L;
    if (A286104_d(L) < A286104_d(R))
        return (seq_t)L;
    return (seq_t)R;
}
