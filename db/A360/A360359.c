/*
 * A360359  Numbers k such that A360331(k) = A360331(k+1).
 * https://oeis.org/A360359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- a segmented sieve computes A360331(k) = Product_{p^e
 *            || k, p not a prime-indexed prime} sigma(p^e) for consecutive k,
 *            and collects every k with A360331(k) = A360331(k+1)
 * Range:     u8 n <= 1, u16 n <= 26, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 913 (2026-11-10)
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

#define A360359_SEG 65536

static uint64_t *A360359_terms = NULL;
static size_t A360359_cnt = 0, A360359_cap = 0;
static uint64_t A360359_next = 1;   /* next x to process */
static uint64_t A360359_fprev = 0;  /* f(next-1) */
static uint64_t A360359_pc = 0;     /* number of primes seen so far */

static unsigned char *A360359_isp = NULL;  /* bitset: x is prime */
static unsigned char *A360359_pip = NULL;  /* bitset: x is a prime-indexed prime */
static uint64_t A360359_bits = 0;          /* bitset capacity (numbers) */

static uint64_t *A360359_bp = NULL;        /* base primes */
static unsigned char *A360359_bpip = NULL; /* base prime is prime-indexed */
static size_t A360359_bpc = 0;
static uint64_t A360359_blim = 0;          /* all primes <= blim are in bp */

static uint32_t A360359_rem[A360359_SEG];
static uint64_t A360359_val[A360359_SEG];
static unsigned char A360359_comp[A360359_SEG];

#define A360359_GET(b, x) (((b)[(x) >> 3] >> ((x) & 7)) & 1)
#define A360359_SET(b, x) ((b)[(x) >> 3] |= (unsigned char)(1u << ((x) & 7)))

static void A360359_grow_bits(uint64_t need)
{
    if (need < A360359_bits) return;
    uint64_t nb = A360359_bits * 2;
    if (nb < need + 1) nb = need + 1;
    if (nb < ((uint64_t)1 << 20)) nb = (uint64_t)1 << 20;
    nb = (nb + 7) & ~(uint64_t)7;
    size_t oldb = (size_t)(A360359_bits / 8), newb = (size_t)(nb / 8);
    A360359_isp = (unsigned char *)realloc(A360359_isp, newb);
    A360359_pip = (unsigned char *)realloc(A360359_pip, newb);
    memset(A360359_isp + oldb, 0, newb - oldb);
    memset(A360359_pip + oldb, 0, newb - oldb);
    A360359_bits = nb;
}

static void A360359_ensure_base(uint64_t hi)
{
    if (A360359_blim > 0 && A360359_blim * A360359_blim >= hi) return;
    uint64_t lim = A360359_blim * 2;
    uint64_t s = 1;
    while ((s + 1) * (s + 1) <= hi) s++;
    if (lim < s + 2) lim = s + 2;
    if (lim < 1024) lim = 1024;
    if (A360359_bp) free(A360359_bp);
    if (A360359_bpip) free(A360359_bpip);
    A360359_bp = NULL;
    build_primes_upto((idx_t)lim, &A360359_bp, &A360359_bpc);
    A360359_bpip = (unsigned char *)malloc(A360359_bpc + 1);
    for (size_t k = 0; k < A360359_bpc; k++)
        A360359_bpip[k] = is_prime((idx_t)(k + 1)) ? 1 : 0;
    A360359_blim = lim - 1;
}

static void A360359_push(uint64_t t)
{
    if (A360359_cnt == A360359_cap) {
        A360359_cap = A360359_cap ? A360359_cap * 2 : 1024;
        A360359_terms = (uint64_t *)realloc(A360359_terms, A360359_cap * sizeof(uint64_t));
    }
    A360359_terms[A360359_cnt++] = t;
}

static void A360359_segment(void)
{
    uint64_t lo = A360359_next, hi = lo + A360359_SEG - 1;
    size_t i;
    A360359_ensure_base(hi);
    A360359_grow_bits(hi);
    for (i = 0; i < A360359_SEG; i++) {
        A360359_rem[i] = (uint32_t)(lo + i);
        A360359_val[i] = 1;
        A360359_comp[i] = 0;
    }
    for (size_t k = 0; k < A360359_bpc; k++) {
        uint64_t p = A360359_bp[k];
        if (p * p > hi) break;
        uint64_t start = ((lo + p - 1) / p) * p;
        uint64_t sq = p * p;
        bool pip = A360359_bpip[k] != 0;
        uint32_t p32 = (uint32_t)p;
        for (uint64_t m = start; m <= hi; m += p) {
            size_t j = (size_t)(m - lo);
            uint32_t r = A360359_rem[j] / p32;
            uint64_t pw = p, s = 1 + p;
            while (r % p32 == 0) { r /= p32; pw *= p; s += pw; }
            A360359_rem[j] = r;
            if (!pip) A360359_val[j] *= s;
            if (m >= sq) A360359_comp[j] = 1;
        }
    }
    for (i = 0; i < A360359_SEG; i++) {
        uint64_t x = lo + i;
        uint64_t r = A360359_rem[i];
        if (x >= 2 && !A360359_comp[i]) {
            A360359_pc++;
            A360359_SET(A360359_isp, x);
            uint64_t c = A360359_pc;   /* c < x, so its primality is known */
            if (c >= 2 && A360359_GET(A360359_isp, c)) A360359_SET(A360359_pip, x);
        }
        if (r > 1 && !A360359_GET(A360359_pip, r)) A360359_val[i] *= (r + 1);
        uint64_t f = A360359_val[i];
        if (x >= 2 && f == A360359_fprev) A360359_push(x - 1);
        A360359_fprev = f;
    }
    A360359_next = hi + 1;
}

seq_t a(idx_t n)
{
    while ((uint64_t)A360359_cnt < (uint64_t)n) A360359_segment();
    return (seq_t)A360359_terms[n - 1];
}
