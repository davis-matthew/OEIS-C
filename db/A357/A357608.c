/*
 * A357608  Numbers k such that k and k+1 are both in A357605.
 * https://oeis.org/A357608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve search: k such that A162296(k) > 2*k and A162296(k+1) >
 *            2*(k+1), with A162296(k) = sigma(k) - Product_{p|k} (1+p),
 *            computed by a segmented multiplicative sieve without divisions
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 49 terms through n = 9641 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A357608_SEG 32768u

static uint64_t *A357608_found = 0;
static size_t A357608_cnt = 0, A357608_cap = 0;
static uint64_t A357608_next = 1;     /* next k to examine */
static bool A357608_prev = false;     /* flag of A357608_next - 1 */

static uint64_t *A357608_pr = 0;
static size_t A357608_prn = 0;
static uint64_t A357608_prlim = 0;

static uint64_t A357608_c[A357608_SEG];    /* prod (1+p) over exponent-1 primes */
static uint64_t A357608_P[A357608_SEG];    /* product of found prime powers */
static uint64_t A357608_S2[A357608_SEG];   /* prod sigma(p^e) over e>=2 */
static uint64_t A357608_F2[A357608_SEG];   /* prod (1+p) over e>=2 */

static void A357608_ensure_primes(uint64_t lim)
{
    if (lim <= A357608_prlim) return;
    uint64_t L = A357608_prlim ? A357608_prlim : 1024;
    while (L < lim) L *= 2;
    char *comp = (char *)calloc((size_t)L + 1, 1);
    free(A357608_pr);
    A357608_pr = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)L / 2 + 16));
    A357608_prn = 0;
    for (uint64_t i = 2; i <= L; ++i) {
        if (!comp[i]) {
            A357608_pr[A357608_prn++] = i;
            for (uint64_t j = i * i; j <= L; j += i) comp[j] = 1;
        }
    }
    free(comp);
    A357608_prlim = L;
}

static void A357608_segment(void)
{
    uint64_t lo = A357608_next;
    uint64_t hi = lo + A357608_SEG;       /* exclusive */
    uint64_t s = 1;
    while (s * s < hi) ++s;
    A357608_ensure_primes(s + 1);

    uint64_t *c = A357608_c, *P = A357608_P, *S2 = A357608_S2, *F2 = A357608_F2;
    for (uint64_t i = 0; i < A357608_SEG; ++i) {
        c[i] = 1; P[i] = 1; S2[i] = 1; F2[i] = 1;
    }
    for (size_t pi = 0; pi < A357608_prn; ++pi) {
        uint64_t p = A357608_pr[pi];
        if (p * p >= hi) break;
        uint64_t p1 = p + 1;
        /* exact exponent 1 */
        {
            uint64_t m0 = ((lo + p - 1) / p) * p;
            uint64_t cnt = (m0 / p) % p;
            for (uint64_t m = m0; m < hi; m += p) {
                if (cnt) {
                    uint64_t i = m - lo;
                    c[i] *= p1;
                    P[i] *= p;
                }
                if (++cnt == p) cnt = 0;
            }
        }
        /* exact exponent e >= 2 */
        uint64_t pe = p * p;
        uint64_t spe = 1 + p + pe;
        while (pe < hi) {
            uint64_t m0 = ((lo + pe - 1) / pe) * pe;
            uint64_t cnt = (m0 / pe) % p;
            for (uint64_t m = m0; m < hi; m += pe) {
                if (cnt) {
                    uint64_t i = m - lo;
                    S2[i] *= spe;
                    F2[i] *= p1;
                    P[i] *= pe;
                }
                if (++cnt == p) cnt = 0;
            }
            if (pe > hi / p) break;
            pe *= p;
            spe += pe;
        }
    }
    for (uint64_t i = 0; i < A357608_SEG; ++i) {
        uint64_t k = lo + i;
        bool cur = false;
        if (S2[i] != F2[i]) {
            uint64_t diff = S2[i] - F2[i];
            uint64_t val = c[i] * diff;
            if (P[i] == k) {
                cur = val > 2 * k;
            } else {
                /* one remaining prime r > sqrt; val*(r+1)/r <= 1.5*val */
                if (3 * val > 4 * P[i]) {
                    uint64_t r = k / P[i];
                    cur = val * (r + 1) > 2 * k;
                }
            }
        }
        if (A357608_prev && cur) {
            if (A357608_cnt == A357608_cap) {
                A357608_cap = A357608_cap ? A357608_cap * 2 : 1024;
                A357608_found = (uint64_t *)realloc(A357608_found, sizeof(uint64_t) * A357608_cap);
            }
            A357608_found[A357608_cnt++] = k - 1;
        }
        A357608_prev = cur;
    }
    A357608_next = hi;
}

seq_t a(idx_t n)
{
    while (A357608_cnt < (size_t)n) A357608_segment();
    return (seq_t)A357608_found[(size_t)n - 1];
}
