/*
 * A343904  Composite numbers k such that k is divisible by A318996(k).
 * https://oeis.org/A343904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search over composite k, computing A318996(k) = Sum_{d|k}
 *            (sigma(k) mod d) with a segmented sieve (sigma by prime sieving,
 *            divisor pairs d, k/d with d <= sqrt(k)), keeping k when
 *            A318996(k) divides k
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define A343904_SEG (1u << 16)

static uint32_t A343904_primes[7000];
static size_t A343904_np = 0;
static bool A343904_inited = false;

static uint64_t *A343904_terms = 0;
static size_t A343904_cnt = 0, A343904_cap = 0;
static uint64_t A343904_hi = 1; /* all k < A343904_hi have been scanned */

static uint32_t A343904_rem[A343904_SEG];
static uint64_t A343904_sig[A343904_SEG];
static uint64_t A343904_S[A343904_SEG];

static void A343904_init(void)
{
    static unsigned char comp[65537];
    for (uint32_t i = 2; i <= 65536; ++i) {
        if (!comp[i]) {
            A343904_primes[A343904_np++] = i;
            for (uint32_t j = i * i; j <= 65536 && i <= 256; j += i)
                comp[j] = 1;
        }
    }
    A343904_inited = true;
}

static void A343904_push(uint64_t k)
{
    if (A343904_cnt == A343904_cap) {
        size_t nc = A343904_cap ? A343904_cap * 2 : 64;
        uint64_t *t = (uint64_t *)realloc(A343904_terms, nc * sizeof(uint64_t));
        if (!t) return;
        A343904_terms = t;
        A343904_cap = nc;
    }
    A343904_terms[A343904_cnt++] = k;
}

static void A343904_segment(uint64_t lo, uint64_t hi)
{
    size_t len = (size_t)(hi - lo);
    uint32_t *rem = A343904_rem;
    uint64_t *sig = A343904_sig;
    uint64_t *S = A343904_S;

    for (size_t i = 0; i < len; ++i) {
        rem[i] = (uint32_t)(lo + i);
        sig[i] = 1;
        S[i] = 0;
    }

    /* sigma via prime sieving */
    for (size_t pi = 0; pi < A343904_np; ++pi) {
        uint64_t p = A343904_primes[pi];
        if (p * p >= hi) break;
        uint64_t k0 = ((lo + p - 1) / p) * p;
        for (uint64_t k = k0; k < hi; k += p) {
            size_t i = (size_t)(k - lo);
            uint32_t r = rem[i];
            uint64_t pp = 1, s = 1;
            do { r /= (uint32_t)p; pp *= p; s += pp; } while (r % (uint32_t)p == 0);
            rem[i] = r;
            sig[i] *= s;
        }
    }
    for (size_t i = 0; i < len; ++i)
        if (rem[i] > 1) sig[i] *= (uint64_t)rem[i] + 1;

    /* divisor pairs (d, k/d) with d*d <= k */
    for (uint64_t d = 1; d * d < hi; ++d) {
        uint64_t kstart = ((lo + d - 1) / d) * d;
        if (kstart < d * d) kstart = d * d;
        if (kstart >= hi) continue;
        uint64_t q = kstart / d;
        uint64_t M = UINT64_C(0xFFFFFFFFFFFFFFFF) / d + 1;
        uint32_t d32 = (uint32_t)d;
        for (uint64_t k = kstart; k < hi; k += d, ++q) {
            size_t i = (size_t)(k - lo);
            uint64_t s = sig[i];
            uint64_t add;
            if ((s >> 32) == 0) {
                uint64_t low = M * s;
                add = (uint64_t)(((unsigned __int128)low * d32) >> 64);
                if (q != d) add += (uint32_t)s % (uint32_t)q;
            } else {
                add = s % d;
                if (q != d) add += s % q;
            }
            S[i] += add;
        }
    }

    for (size_t i = 0; i < len; ++i) {
        uint64_t k = lo + i;
        if (k < 4) continue;
        if (sig[i] == k + 1) continue; /* prime */
        uint64_t s = S[i];
        if (s > 0 && s <= k && k % s == 0)
            A343904_push(k);
    }
}

seq_t a(idx_t n)
{
    if (!A343904_inited) A343904_init();
    const uint64_t CAP = UINT64_C(0xFFFFFFFF);
    while ((idx_t)A343904_cnt < n && A343904_hi < CAP) {
        uint64_t lo = A343904_hi;
        uint64_t hi = lo + A343904_SEG;
        if (hi > CAP) hi = CAP;
        A343904_segment(lo, hi);
        A343904_hi = hi;
    }
    if ((idx_t)A343904_cnt < n) return (seq_t)0;
    return (seq_t)A343904_terms[n - 1];
}
