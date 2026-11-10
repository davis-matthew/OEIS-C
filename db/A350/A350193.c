/*
 * A350193  Indices that give distinct values of A350090 in the order in which
 *          they appear.
 * https://oeis.org/A350193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A350090(m) = tau(3*m^2+3*m+1) - 1 (all prime factors of the
 *            centered hexagonal number are 1 mod 3, so the x^2+3y^2
 *            representation count reduces to its number of divisors). a(n) =
 *            the n-th m >= 0 where this value appears for the first time.
 *            Computed by a segmented sieve that factors 3m^2+3m+1
 * Range:     u8 n <= 8, u16 n <= 22, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-10)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/isqrt_u64.h"

typedef struct { uint64_t p, n1, n2; } A350193_prime;

#define A350193_BLK ((uint64_t)1 << 18)

static A350193_prime *A350193_pr = 0;
static size_t A350193_prcnt = 0, A350193_prcap = 0;
static uint64_t A350193_plimit = 1;
static uint64_t *A350193_res = 0;
static size_t A350193_rescnt = 0, A350193_rescap = 0;
static bool *A350193_seen = 0;
static size_t A350193_seencap = 0;
static uint64_t A350193_nextL = 0;
static uint64_t *A350193_rem = 0;
static uint32_t *A350193_tau = 0;

static uint64_t A350193_pm(uint64_t b, uint64_t e, uint64_t p)
{
    uint64_t r = 1;
    b %= p;
    while (e) {
        if (e & 1) r = r * b % p;
        b = b * b % p;
        e >>= 1;
    }
    return r;
}

static uint64_t A350193_first_ge(uint64_t r, uint64_t p, uint64_t L)
{
    if (r >= L) return r;
    return r + ((L - r + p - 1) / p) * p;
}

static void A350193_add_prime(uint64_t p, uint64_t L)
{
    uint64_t g, w = 1, w2, m1, m2;
    if (p % 3 != 1) return;
    for (g = 2; ; ++g) {
        w = A350193_pm(g, (p - 1) / 3, p);
        if (w != 1) break;
    }
    w2 = w * w % p;
    m1 = A350193_pm(w - 1, p - 2, p);
    m2 = A350193_pm(w2 - 1, p - 2, p);
    if (A350193_prcnt == A350193_prcap) {
        size_t nc = A350193_prcap ? A350193_prcap * 2 : 1024;
        A350193_pr = (A350193_prime *)realloc(A350193_pr, nc * sizeof(A350193_prime));
        A350193_prcap = nc;
    }
    A350193_pr[A350193_prcnt].p = p;
    A350193_pr[A350193_prcnt].n1 = A350193_first_ge(m1, p, L);
    A350193_pr[A350193_prcnt].n2 = A350193_first_ge(m2, p, L);
    ++A350193_prcnt;
}

static void A350193_extend_primes(uint64_t newlimit, uint64_t L)
{
    uint64_t base = isqrt_u64(newlimit) + 1;
    unsigned char *bs = (unsigned char *)calloc((size_t)base + 1, 1);
    uint64_t *bp = 0;
    size_t bcnt = 0, bcap = 0;
    uint64_t i, j, lo;
    const uint64_t S = (uint64_t)1 << 18;
    unsigned char *mark = (unsigned char *)malloc((size_t)S);

    for (i = 2; i <= base; ++i) {
        if (bs[i]) continue;
        if (bcnt == bcap) {
            bcap = bcap ? bcap * 2 : 256;
            bp = (uint64_t *)realloc(bp, bcap * sizeof(uint64_t));
        }
        bp[bcnt++] = i;
        for (j = i * i; j <= base; j += i) bs[j] = 1;
    }
    for (lo = A350193_plimit + 1; lo <= newlimit; lo += S) {
        uint64_t hi = lo + S - 1;
        size_t k;
        if (hi > newlimit) hi = newlimit;
        memset(mark, 0, (size_t)(hi - lo + 1));
        for (k = 0; k < bcnt; ++k) {
            uint64_t q = bp[k], st;
            if (q * q > hi) break;
            st = ((lo + q - 1) / q) * q;
            if (st < q * q) st = q * q;
            for (j = st; j <= hi; j += q) mark[j - lo] = 1;
        }
        for (j = lo; j <= hi; ++j)
            if (j >= 2 && !mark[j - lo]) A350193_add_prime(j, L);
    }
    A350193_plimit = newlimit;
    free(mark);
    free(bp);
    free(bs);
}

static void A350193_block(void)
{
    uint64_t L = A350193_nextL, B = A350193_BLK, E = L + B;
    uint64_t Hmax = 3 * (E - 1) * E + 1;
    uint64_t P = isqrt_u64(Hmax);
    uint64_t j;
    size_t k;

    if (!A350193_rem) {
        A350193_rem = (uint64_t *)malloc((size_t)B * sizeof(uint64_t));
        A350193_tau = (uint32_t *)malloc((size_t)B * sizeof(uint32_t));
    }
    if (P > A350193_plimit) {
        uint64_t nl = P * 2;
        if (nl < 65536) nl = 65536;
        A350193_extend_primes(nl, L);
    }
    for (j = 0; j < B; ++j) {
        uint64_t m = L + j;
        A350193_rem[j] = 3 * m * (m + 1) + 1;
        A350193_tau[j] = 1;
    }
    for (k = 0; k < A350193_prcnt; ++k) {
        uint64_t p = A350193_pr[k].p;
        int t;
        for (t = 0; t < 2; ++t) {
            uint64_t x = t ? A350193_pr[k].n2 : A350193_pr[k].n1;
            while (x < E) {
                uint64_t jj = x - L;
                uint32_t e = 0;
                while (A350193_rem[jj] % p == 0) {
                    A350193_rem[jj] /= p;
                    ++e;
                }
                A350193_tau[jj] *= (e + 1);
                x += p;
            }
            if (t) A350193_pr[k].n2 = x; else A350193_pr[k].n1 = x;
        }
    }
    for (j = 0; j < B; ++j) {
        uint32_t v;
        if (A350193_rem[j] > 1) A350193_tau[j] *= 2;
        v = A350193_tau[j] - 1;
        if ((size_t)v >= A350193_seencap) {
            size_t nc = A350193_seencap ? A350193_seencap : 256;
            while (nc <= (size_t)v) nc *= 2;
            A350193_seen = (bool *)realloc(A350193_seen, nc * sizeof(bool));
            memset(A350193_seen + A350193_seencap, 0, (nc - A350193_seencap) * sizeof(bool));
            A350193_seencap = nc;
        }
        if (!A350193_seen[v]) {
            A350193_seen[v] = true;
            if (A350193_rescnt == A350193_rescap) {
                A350193_rescap = A350193_rescap ? A350193_rescap * 2 : 64;
                A350193_res = (uint64_t *)realloc(A350193_res, A350193_rescap * sizeof(uint64_t));
            }
            A350193_res[A350193_rescnt++] = L + j;
        }
    }
    A350193_nextL = E;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    while ((idx_t)A350193_rescnt < n)
        A350193_block();
    return (seq_t)A350193_res[n - 1];
}
