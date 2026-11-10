/*
 * A383547  Numbers k such that A383546(k) > A383546(m) for all m < k.
 * https://oeis.org/A383547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. a(n) = n-th record position of A383546, where A383546(k)
 *            = min over primes p not dividing k of ((k^(-1) mod p^2) - 1). A
 *            k that beats record value R must be divisible by every prime p
 *            with p^2 <= R+1. Candidates are tested with a modular-inverse
 *            check for small primes, then by testing i*k...
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 28, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define A383547_PLIM 2650000u

static uint32_t *A383547_pr = 0;
static size_t A383547_np = 0;

static void A383547_sieve(void)
{
    if (A383547_pr) return;
    unsigned char *c = (unsigned char *)malloc(A383547_PLIM + 1);
    for (uint32_t i = 0; i <= A383547_PLIM; ++i) c[i] = 1;
    c[0] = c[1] = 0;
    for (uint32_t i = 2; (uint64_t)i * i <= A383547_PLIM; ++i)
        if (c[i])
            for (uint32_t j = i * i; j <= A383547_PLIM; j += i) c[j] = 0;
    size_t cntp = 0;
    for (uint32_t i = 2; i <= A383547_PLIM; ++i) if (c[i]) ++cntp;
    A383547_pr = (uint32_t *)malloc(sizeof(uint32_t) * cntp);
    for (uint32_t i = 2; i <= A383547_PLIM; ++i) if (c[i]) A383547_pr[A383547_np++] = i;
    free(c);
}

static uint64_t A383547_isqrt(uint64_t x)
{
    if (x < 2) return x;
    int b = 0;
    uint64_t t = x;
    while (t) { ++b; t >>= 1; }
    uint64_t r = (uint64_t)1 << ((b + 1) / 2);
    uint64_t y = (r + x / r) / 2;
    while (y < r) { r = y; y = (r + x / r) / 2; }
    return r;
}

static long long A383547_inv(long long u, long long m)
{
    long long g = m, x = 0, x1 = 1, r = u % m;
    while (r != 0) {
        long long qq = g / r, tmp;
        tmp = g - qq * r; g = r; r = tmp;
        tmp = x - qq * x1; x = x1; x1 = tmp;
    }
    x %= m;
    if (x < 0) x += m;
    return x;
}

/* full squarefree test of N (N < 2^64) */
static bool A383547_sf(uint64_t N)
{
    for (size_t j = 0; j < A383547_np; ++j) {
        uint64_t p = A383547_pr[j];
        if ((unsigned __int128)p * p * p > N) break;
        if (N % p == 0) {
            N /= p;
            if (N % p == 0) return false;
        }
    }
    if (N > 1) {
        uint64_t s = A383547_isqrt(N);
        if (s * s == N) return false;
    }
    return true;
}

/* returns 1 if i*k-1 squarefree for all 1<=i<=T, 0 if not, -1 on overflow */
static int A383547_check(uint64_t k, uint64_t T)
{
    unsigned __int128 nm = (unsigned __int128)T * k - 1;
    if (nm > (unsigned __int128)UINT64_MAX) return -1;
    uint64_t Nmax = (uint64_t)nm;
    uint64_t sq = A383547_isqrt(Nmax);
    bool deep = true;
    for (size_t j = 0; j < A383547_np; ++j) {
        uint64_t p = A383547_pr[j];
        if (p > sq) { deep = false; break; }
        if (k % p == 0) continue;
        uint64_t pp = p * p;
        long long iv = A383547_inv((long long)(k % pp), (long long)pp);
        if ((uint64_t)iv <= T) return 0;
    }
    if (deep) {
        /* all primes up to PLIM have been excluded as square divisors */
        for (uint64_t i = 1; i <= T; ++i) {
            uint64_t N = i * k - 1;
            for (size_t j = 0; j < A383547_np; ++j) {
                uint64_t p = A383547_pr[j];
                if ((unsigned __int128)p * p * p > N) break;
                if (N % p == 0) N /= p;
            }
            if (N > 1) {
                uint64_t s = A383547_isqrt(N);
                if (s * s == N) return 0;
            }
        }
    }
    return 1;
}

seq_t a(idx_t n)
{
    static uint64_t recs[256];
    static idx_t cnt = 0;
    static uint64_t kcur = 0;
    static uint64_t R = 0;
    static bool stop = false;

    A383547_sieve();
    if (cnt == 0) { recs[0] = 1; cnt = 1; kcur = 1; R = 0; }

    while (cnt < n && cnt < 256 && !stop) {
        uint64_t T = R + 1;
        unsigned __int128 P = 1;
        for (size_t j = 0; j < A383547_np; ++j) {
            uint64_t p = A383547_pr[j];
            if (p * p > T) break;
            P *= p;
            if (P > (unsigned __int128)UINT64_MAX) break;
        }
        if (P > (unsigned __int128)UINT64_MAX) { stop = true; break; }
        uint64_t P64 = (uint64_t)P;
        unsigned __int128 t = (unsigned __int128)(kcur / P64) + 1;
        bool found = false;
        for (;;) {
            unsigned __int128 kk = (unsigned __int128)P64 * t;
            if (kk > (unsigned __int128)UINT64_MAX) break;
            uint64_t k = (uint64_t)kk;
            int r = A383547_check(k, T);
            if (r < 0) break;
            if (r == 1) {
                uint64_t i = T + 1;
                bool ovf = false;
                for (;;) {
                    unsigned __int128 N = (unsigned __int128)i * k - 1;
                    if (N > (unsigned __int128)UINT64_MAX) { ovf = true; break; }
                    if (!A383547_sf((uint64_t)N)) break;
                    ++i;
                }
                if (ovf) break;
                R = i - 1;
                kcur = k;
                recs[cnt++] = k;
                found = true;
                break;
            }
            ++t;
        }
        if (!found) { stop = true; break; }
    }
    if (n - 1 >= cnt) return (seq_t)0;
    return (seq_t)recs[n - 1];
}
