/*
 * A251241  Indices of prime powers in A098550.
 * https://oeis.org/A251241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search - generate A098550 (Yellowstone permutation:
 *            a(1..3)=1,2,3; a(k) = least unused m with gcd(m,a(k-1))=1 and
 *            gcd(m,a(k-2))>1) and list, in increasing order, the indices k
 *            where A098550(k) is 1 or a prime power
 * Range:     u8 n <= 46, u16 n <= 3447, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 74 terms through n = 4619 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define A251241_CAP (1u << 23)

static unsigned char *A251241_used = 0;
static uint32_t *A251241_ptr = 0;   /* ptr[p]: no unused multiple of p below it */
static uint64_t *A251241_idx = 0;
static size_t A251241_cnt = 0, A251241_cap = 0;
static uint64_t A251241_p1, A251241_p2, A251241_k;

static uint64_t A251241_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static bool A251241_pp(uint64_t v)
{
    if (v == 1) return true;
    for (uint64_t d = 2; d * d <= v; d++) {
        if (v % d == 0) {
            while (v % d == 0) v /= d;
            return v == 1;
        }
    }
    return true; /* prime */
}

static void A251241_push(uint64_t v)
{
    if (A251241_pp(v)) {
        if (A251241_cnt == A251241_cap) {
            A251241_cap = A251241_cap ? A251241_cap * 2 : 1024;
            A251241_idx = (uint64_t *)realloc(A251241_idx, A251241_cap * sizeof(uint64_t));
        }
        A251241_idx[A251241_cnt++] = A251241_k;
    }
}

static void A251241_init(void)
{
    A251241_used = (unsigned char *)calloc(A251241_CAP, 1);
    A251241_ptr = (uint32_t *)calloc(A251241_CAP, sizeof(uint32_t));
    A251241_k = 1; A251241_used[1] = 1; A251241_push(1);
    A251241_k = 2; A251241_used[2] = 1; A251241_push(2);
    A251241_k = 3; A251241_used[3] = 1; A251241_push(3);
    A251241_p2 = 2; A251241_p1 = 3;
}

static uint64_t A251241_try_prime(uint64_t p, uint64_t best)
{
    uint64_t m = A251241_ptr[p] ? A251241_ptr[p] : p;
    while (m < A251241_CAP && A251241_used[m]) m += p;
    if (m < A251241_CAP) A251241_ptr[p] = (uint32_t)m;   /* first unused multiple */
    while (m < A251241_CAP && (best == 0 || m < best)) {
        if (!A251241_used[m] && A251241_gcd(m, A251241_p1) == 1) return m;
        m += p;
    }
    return 0;
}

static void A251241_step(void)
{
    uint64_t x = A251241_p2, best = 0, c;
    for (uint64_t p = 2; p * p <= x; p++) {
        if (x % p) continue;
        while (x % p == 0) x /= p;
        c = A251241_try_prime(p, best);
        if (c && (best == 0 || c < best)) best = c;
    }
    if (x > 1) {
        c = A251241_try_prime(x, best);
        if (c && (best == 0 || c < best)) best = c;
    }
    A251241_used[best] = 1;
    A251241_k++;
    A251241_push(best);
    A251241_p2 = A251241_p1;
    A251241_p1 = best;
}

seq_t a(idx_t n)
{
    if (!A251241_used) A251241_init();
    while (A251241_cnt < (size_t)n) A251241_step();
    return (seq_t)A251241_idx[n - 1];
}
