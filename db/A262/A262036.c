/*
 * A262036  a(n) is the least k such that A261865(k) = A005117(n).
 * https://oeis.org/A262036
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   search -- a(n) is the least k whose A261865(k) equals
 *            A005117(n). k is scanned upward. For each k, the code finds the
 *            smallest squarefree j >= 2 for which some multiple m*sqrt(j)
 *            lies strictly between k and k+1. That happens exactly when
 *            frac(k/sqrt(j)) > 1 - 1/sqrt(j). The test uses 64-bit fixed...
 * Range:     u8 n <= 9, u16 n <= 31, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: standard, pass, 58 terms through n = 88 (2026-11-07)
 * Helpers:   isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isqrt_u128.h"

static uint64_t *A262036_sf = 0;    /* squarefree numbers >= 2 */
static uint64_t *A262036_fx = 0;    /* floor(2^64 / sqrt(sf[p])) approx */
static uint64_t *A262036_first = 0; /* least k with A261865(k) = sf[p] */
static size_t A262036_cnt = 0, A262036_cap = 0;

static bool A262036_sqfree(uint64_t k)
{
    if (k % 4 == 0) return false;
    for (uint64_t i = 3; i * i <= k; i += 2)
        if (k % (i * i) == 0) return false;
    return true;
}

static void A262036_ensure(size_t need)
{
    if (need > A262036_cap) {
        size_t nc = A262036_cap ? A262036_cap : 64;
        while (nc < need) nc *= 2;
        A262036_sf = (uint64_t *)realloc(A262036_sf, nc * sizeof(uint64_t));
        A262036_fx = (uint64_t *)realloc(A262036_fx, nc * sizeof(uint64_t));
        A262036_first = (uint64_t *)realloc(A262036_first, nc * sizeof(uint64_t));
        for (size_t i = A262036_cap; i < nc; i++) A262036_first[i] = 0;
        A262036_cap = nc;
    }
    while (A262036_cnt < need) {
        uint64_t v = A262036_cnt ? A262036_sf[A262036_cnt - 1] : 1;
        do { v++; } while (!A262036_sqfree(v));
        A262036_sf[A262036_cnt] = v;
        unsigned __int128 top = ~(unsigned __int128)0;
        A262036_fx[A262036_cnt] = (uint64_t)isqrt_u128(top / v);
        A262036_cnt++;
    }
}

/* exact: is there m with k^2 < m^2*j < (k+1)^2 ? */
static bool A262036_exact(uint64_t k, uint64_t j)
{
    unsigned __int128 k2 = (unsigned __int128)k * k;
    unsigned __int128 m = isqrt_u128(k2 / j) + 1;
    unsigned __int128 k1 = (unsigned __int128)k + 1;
    return m * m * j < k1 * k1;
}

/* does some multiple of sqrt(j) lie strictly between k and k+1? */
static bool A262036_hit(uint64_t k, uint64_t j, uint64_t A)
{
    uint64_t F = k * A;              /* ~ frac(k/sqrt(j)) * 2^64 */
    uint64_t C = (uint64_t)0 - A;    /* ~ (1 - 1/sqrt(j)) * 2^64 */
    uint64_t mg = 2 * k + 64;        /* error bound margin */
    uint64_t d = F - C;
    if (k > ((uint64_t)1 << 60) || F < mg || F > UINT64_MAX - mg ||
        d < mg || d > UINT64_MAX - mg)
        return A262036_exact(k, j);
    return F >= C;
}

seq_t a(idx_t n)
{
    static uint64_t nextk = 1;
    size_t t = (size_t)(n - 2); /* index of A005117(n) among squarefree >= 2 */
    A262036_ensure(t + 1);

    while (A262036_first[t] == 0) {
        uint64_t k = nextk;
        if (A262036_hit(k, A262036_sf[0], A262036_fx[0])) {
            if (A262036_first[0] == 0) A262036_first[0] = k;
            nextk = k + 1;
            continue;
        }
        for (size_t p = 1;; p++) {
            A262036_ensure(p + 1);
            if (A262036_hit(k, A262036_sf[p], A262036_fx[p])) {
                if (A262036_first[p] == 0) A262036_first[p] = k;
                break;
            }
        }
        /* k is not hit by sqrt(2); the next such k is k+3 or k+4 */
        nextk = k + 3;
    }
    return (seq_t)A262036_first[t];
}
