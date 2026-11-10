/*
 * A341632  Primes in A073837.
 * https://oeis.org/A341632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A073837(A341631(n)); computed by a sieve over k, keeping
 *            S(k) = A073837(k) = sum of primes p with k <= p <= 2k up to date
 *            as k grows, and collecting the values of S(k) that are prime
 *            (Miller-Rabin test)
 * Range:     u8 n <= 6, u16 n <= 70, u32 n <= 9447, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 8863 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A341632_res = NULL;
static size_t A341632_cnt = 0;
static size_t A341632_cap = 0;
static idx_t A341632_K = 0;

static uint64_t A341632_mulmod(uint64_t x, uint64_t y, uint64_t m)
{
    return (uint64_t)(((unsigned __int128)x * y) % m);
}

static uint64_t A341632_powmod(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = A341632_mulmod(r, b, m);
        b = A341632_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool A341632_isprime(uint64_t n)
{
    static const uint64_t small[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    if (n < 2) return false;
    for (int i = 0; i < 12; ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    for (int i = 0; i < 12; ++i) {
        uint64_t x = A341632_powmod(small[i], d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; ++r) {
            x = A341632_mulmod(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

static void A341632_build(idx_t K)
{
    size_t lim = (size_t)(2 * K + 3);
    uint8_t *isp = (uint8_t *)malloc(lim + 1);
    if (!isp) exit(1);
    for (size_t i = 0; i <= lim; ++i) isp[i] = 1;
    isp[0] = 0;
    isp[1] = 0;
    for (size_t p = 2; p * p <= lim; ++p) {
        if (!isp[p]) continue;
        for (size_t j = p * p; j <= lim; j += p) isp[j] = 0;
    }
    A341632_cnt = 0;
    uint64_t S = 2; /* primes in [1,2] */
    for (idx_t k = 1; k <= K; ++k) {
        if (k > 1) {
            if (isp[k - 1]) S -= (uint64_t)(k - 1);
            if (isp[2 * k - 1]) S += (uint64_t)(2 * k - 1);
            if (isp[2 * k]) S += (uint64_t)(2 * k);
        }
        if (A341632_isprime(S)) {
            if (A341632_cnt >= A341632_cap) {
                size_t nc = A341632_cap ? A341632_cap * 2 : 256;
                seq_t *t = mem_resize(seq_t, A341632_res, nc);
                if (!t) exit(1);
                A341632_res = t;
                A341632_cap = nc;
            }
            A341632_res[A341632_cnt++] = (seq_t)S;
        }
    }
    free(isp);
    A341632_K = K;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t K = A341632_K ? A341632_K : 4096;
    while (A341632_K == 0 || A341632_cnt < (size_t)n) {
        A341632_build(K);
        if (A341632_cnt >= (size_t)n) break;
        K *= 4;
    }
    return A341632_res[n - 1];
}
