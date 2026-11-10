/*
 * A343496  First point of the straight lines in A340649.
 * https://oeis.org/A343496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = smallest k with prime(k+1)-prime(k) = 2n and n*... i.e.
 *            A340649(k) = A141042(k), which is equivalent to 2n*k < prime(k).
 *            Search: by Dusart, x/pi(x) < ln x - 1 for x >= 5393, so no prime
 *            p(k) <= e^(2n+1) can satisfy p(k) > 2n*k; start there, get k via
 *            Lucy_Hedgehog prime counting, then segm...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/is_prime_u64.h"

#define A343496_SEG 262144u

static uint64_t A343496_isqrt(uint64_t x)
{
    uint64_t r = (uint64_t)sqrt((double)x);
    while (r * r > x) r--;
    while ((r + 1) * (r + 1) <= x) r++;
    return r;
}

/* SHARED_HELPER: prime counting function pi(x) via Lucy_Hedgehog method */
static uint64_t A343496_prime_pi(uint64_t x)
{
    if (x < 2) return 0;
    uint64_t r = A343496_isqrt(x);
    int64_t *lo = (int64_t *)malloc(sizeof(int64_t) * (r + 2));
    int64_t *hi = (int64_t *)malloc(sizeof(int64_t) * (r + 2));
    uint64_t i, p;
    int64_t res;
    if (!lo || !hi) abort();
    lo[0] = 0;
    for (i = 1; i <= r; i++) {
        lo[i] = (int64_t)i - 1;
        hi[i] = (int64_t)(x / i) - 1;
    }
    for (p = 2; p <= r; p++) {
        if (lo[p] == lo[p - 1]) continue;
        int64_t sp = lo[p - 1];
        uint64_t p2 = p * p;
        uint64_t end = x / p2;
        if (end > r) end = r;
        for (i = 1; i <= end; i++) {
            uint64_t d = i * p;
            int64_t v = (d <= r) ? hi[d] : lo[x / d];
            hi[i] -= v - sp;
        }
        if (p2 <= r) {
            for (i = r; i >= p2; i--) lo[i] -= lo[i / p] - sp;
        }
    }
    res = hi[1];
    free(lo);
    free(hi);
    return (uint64_t)res;
}

static uint32_t *A343496_base_odd_primes(uint64_t B, size_t *cnt)
{
    unsigned char *c = (unsigned char *)calloc((size_t)B + 1, 1);
    uint32_t *bp;
    size_t n = 0, cap = 1024;
    uint64_t i, j;
    if (!c) abort();
    bp = (uint32_t *)malloc(sizeof(uint32_t) * cap);
    if (!bp) abort();
    for (i = 3; i <= B; i += 2) {
        if (!c[i]) {
            if (n == cap) {
                cap *= 2;
                bp = (uint32_t *)realloc(bp, sizeof(uint32_t) * cap);
                if (!bp) abort();
            }
            bp[n++] = (uint32_t)i;
            for (j = i * i; j <= B; j += 2 * i) c[j] = 1;
        }
    }
    free(c);
    *cnt = n;
    return bp;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    uint64_t g2 = 2ull * (uint64_t)n;
    uint64_t x0 = 2;
    if (2.0 * (double)n + 1.0 >= 9.0)
        x0 = (uint64_t)(exp(2.0 * (double)n + 1.0) * 0.999);

    /* largest prime <= x0 and its index */
    uint64_t prev = x0;
    while (prev > 2 && !is_prime_u64(prev)) prev--;
    uint64_t k = A343496_prime_pi(prev);

    uint64_t lo = prev + 1;
    if ((lo & 1) == 0) lo++;
    if (lo < 3) lo = 3;

    unsigned char *seg = (unsigned char *)malloc(A343496_SEG);
    if (!seg) abort();
    uint64_t B = 0;
    size_t nbp = 0;
    uint32_t *bp = NULL;
    uint64_t result = 0;

    while (!result) {
        uint64_t hi = lo + 2ull * (A343496_SEG - 1);
        if (B * B < hi) {
            B = A343496_isqrt(hi) * 2 + 1000;
            free(bp);
            bp = A343496_base_odd_primes(B, &nbp);
        }
        memset(seg, 1, A343496_SEG);
        for (size_t t = 0; t < nbp; t++) {
            uint64_t p = bp[t];
            uint64_t s;
            if (p * p > hi) break;
            if (p * p >= lo) s = p * p;
            else {
                s = ((lo + p - 1) / p) * p;
                if ((s & 1) == 0) s += p;
            }
            for (uint64_t xi = (s - lo) >> 1; xi < A343496_SEG; xi += p) seg[xi] = 0;
        }
        for (uint32_t i = 0; i < A343496_SEG; i++) {
            if (seg[i]) {
                uint64_t q = lo + 2ull * i;
                if (q - prev == g2 && g2 * k < prev) { result = k; break; }
                prev = q;
                k++;
            }
        }
        lo = hi + 2;
    }
    free(seg);
    free(bp);
    return (seq_t)result;
}
