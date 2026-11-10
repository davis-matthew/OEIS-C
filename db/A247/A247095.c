/*
 * A247095  Smallest number m such that A250030(m) = n.
 * https://oeis.org/A247095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search. a(n) = smallest m with A250030(m) = n. Every such m is
 *            k*P, where P is the largest prime factor of m and k is small,
 *            because A008474(m) >= a(n-1) is required. The code goes through
 *            the primes P in increasing blocks of m, using an odd-only prime
 *            bitmap, and checks the depth of A008474(m)....
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-07)
 * Helpers:   ctz_u64, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"
#include "../utils/ctz_u64.h"

static uint64_t A247095_memo[64];
static uint64_t *A247095_bits = 0;
static uint64_t A247095_lim = 0;          /* all x < lim covered */
static uint32_t *A247095_base = 0;
static size_t A247095_nbase = 0;

static void A247095_ensure_sieve(uint64_t N)
{
    if (N <= A247095_lim) return;
    uint64_t oldlim = A247095_lim;
    uint64_t newlim = oldlim + oldlim / 4;
    if (newlim < N) newlim = N;
    if (newlim < ((uint64_t)1 << 16)) newlim = (uint64_t)1 << 16;
    newlim = (newlim + 127) & ~(uint64_t)127;

    uint64_t s = 1;
    while ((s + 1) * (s + 1) <= newlim) s++;
    s += 2;
    char *c = (char *)calloc((size_t)s + 1, 1);
    uint32_t *bp = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)s / 2 + 8));
    size_t nb = 0;
    for (uint64_t i = 2; i <= s; i++) {
        if (!c[i]) {
            bp[nb++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= s; j += i) c[j] = 1;
        }
    }
    free(c);
    if (A247095_base) free(A247095_base);
    A247095_base = bp;
    A247095_nbase = nb;

    size_t oldw = (size_t)(oldlim / 128), neww = (size_t)(newlim / 128);
    A247095_bits = (uint64_t *)realloc(A247095_bits, sizeof(uint64_t) * (neww + 1));
    for (size_t w = oldw; w <= neww; w++) A247095_bits[w] = 0;

    const uint64_t SEG = (uint64_t)1 << 18;
    char *seg = (char *)malloc((size_t)SEG);
    uint64_t iend = newlim / 2;
    for (uint64_t i0 = oldlim / 2; i0 < iend; i0 += SEG) {
        uint64_t i1 = i0 + SEG;
        if (i1 > iend) i1 = iend;
        memset(seg, 0, (size_t)(i1 - i0));
        uint64_t x0 = 2 * i0 + 1;
        for (size_t t = 1; t < nb; t++) {
            uint64_t p = bp[t];
            if (p * p >= 2 * i1) break;
            uint64_t mm = p * p;
            if (mm < x0) mm = x0;
            uint64_t cc = ((mm + p - 1) / p) * p;
            if (!(cc & 1)) cc += p;
            for (uint64_t j = cc >> 1; j < i1; j += p) seg[j - i0] = 1;
        }
        for (uint64_t j = i0; j < i1; j++)
            if (!seg[j - i0] && j != 0)
                A247095_bits[j >> 6] |= (uint64_t)1 << (j & 63);
    }
    free(seg);
    A247095_lim = newlim;
}

static bool A247095_isprime(uint64_t x)
{
    if (x < A247095_lim) {
        if (x == 2) return true;
        if (!(x & 1)) return false;
        uint64_t j = x >> 1;
        return (A247095_bits[j >> 6] >> (j & 63)) & 1;
    }
    return is_prime_u64(x);
}

/* A008474(v); returns false as soon as A008474(v) < thr is proven */
static bool A247095_fb(uint64_t v, uint64_t thr, uint64_t *out)
{
    if (v < 2) { *out = 0; return 0 >= thr; }
    if (v + 1 < thr) return false;
    uint64_t sum = 0, r = v;
    size_t i = 0;
    uint64_t q = 2;
    for (;;) {
        if (r == 1) break;
        if (A247095_isprime(r)) { sum += r + 1; break; }
        if (i < A247095_nbase) q = A247095_base[i];
        else q = (i == A247095_nbase && A247095_nbase) ? (uint64_t)A247095_base[i - 1] + 2 : q + 2;
        i++;
        /* r composite, all prime factors >= q: A008474(r) <= r/q + q + 64 */
        if (sum + r / q + q + 64 < thr) return false;
        uint64_t rem;
        if (r <= 0xFFFFFFFFull) rem = (uint32_t)r % (uint32_t)q;
        else rem = r % q;
        if (rem == 0) {
            unsigned e = 0;
            while (r % q == 0) { r /= q; e++; }
            sum += q + e;
        }
    }
    *out = sum;
    return sum >= thr;
}

static bool A247095_has_depth(uint64_t v, int d)
{
    while (d > 0) {
        if (v == 5) return false;
        if (v < A247095_memo[d]) return false;
        uint64_t nv;
        if (!A247095_fb(v, A247095_memo[d - 1], &nv)) return false;
        v = nv;
        d--;
    }
    return v == 5;
}

static uint64_t A247095_f_plain(uint64_t m, uint64_t *gpf)
{
    uint64_t s = 0, r = m, g = 1;
    for (uint64_t p = 2; p * p <= r; p++) {
        if (r % p == 0) {
            unsigned e = 0;
            while (r % p == 0) { r /= p; e++; }
            s += p + e;
            g = p;
        }
    }
    if (r > 1) { s += r + 1; g = r; }
    if (gpf) *gpf = g;
    return s;
}

static int A247095_plain_depth(uint64_t m)
{
    int d = 0;
    while (m != 5) {
        m = A247095_f_plain(m, 0);
        d++;
        if (d > 200 || m < 5) return -1;
    }
    return d;
}

static uint64_t A247095_compute(int n)
{
    A247095_memo[0] = 5;
    if (A247095_memo[n]) return A247095_memo[n];
    if (n <= 11) {
        for (uint64_t m = 5;; m++)
            if (A247095_plain_depth(m) == n) { A247095_memo[n] = m; return m; }
    }
    for (int d = 1; d < n; d++) A247095_compute(d);
    uint64_t T = A247095_memo[n - 1];
    uint64_t lo = T - 1, best = 0;
    for (;;) {
        uint64_t hi = lo + lo / 8 + ((uint64_t)1 << 20);
        A247095_ensure_sieve(hi + 128);
        uint64_t Kmax = 16 * hi / (T - 65) + 1;
        for (uint64_t k = 1; k <= Kmax; k++) {
            uint64_t gk = 1, fk = 0;
            if (k > 1) fk = A247095_f_plain(k, &gk);
            uint64_t L = gk;
            if (T - 1 > fk && T - 1 - fk > L) L = T - 1 - fk;
            uint64_t Plow = (lo + k - 1) / k;
            if (Plow < L) Plow = L;
            if (Plow < 2) Plow = 2;
            uint64_t Phigh = (hi - 1) / k;
            if (best && (best - 1) / k < Phigh) Phigh = (best - 1) / k;
            if (Plow > Phigh) continue;
            if (Plow == 2) {
                uint64_t fm = (gk == 2) ? fk + 1 : fk + 3;
                if (fm >= T && A247095_has_depth(fm, n - 1)) {
                    if (!best || 2 * k < best) best = 2 * k;
                    continue;
                }
                Plow = 3;
            }
            uint64_t j = Plow >> 1, jhi = (Phigh - 1) >> 1;
            if (!(Phigh & 1)) jhi = (Phigh - 1) >> 1; else jhi = Phigh >> 1;
            while (j <= jhi) {
                uint64_t w = A247095_bits[j >> 6] >> (j & 63);
                if (!w) { j = (j | 63) + 1; continue; }
                j += (uint64_t)ctz_u64(w);
                if (j > jhi) break;
                uint64_t P = 2 * j + 1;
                uint64_t fm = (P == gk) ? fk + 1 : fk + P + 1;
                if (fm >= T && A247095_has_depth(fm, n - 1)) {
                    uint64_t m = k * P;
                    if (!best || m < best) best = m;
                    break;
                }
                j++;
            }
        }
        if (best) break;
        lo = hi;
    }
    A247095_memo[n] = best;
    return best;
}

seq_t a(idx_t n)
{
    if (n < 1 || n >= 60) return (seq_t)0;
    return (seq_t)A247095_compute((int)n);
}
