/*
 * A382353  Numbers k > 0 such that A006218(k) / A018804(k) is an integer.
 * https://oeis.org/A382353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th k>0 with A018804(k) | A006218(k)... precisely
 *            A006218(k)/A018804(k) an integer; found by a segmented sieve
 *            computing Pillai's function (multiplicative) and the running sum
 *            of d(k)
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A382353_B 65536u

static uint64_t *A382353_found = NULL;
static size_t A382353_cnt = 0, A382353_cap = 0;
static uint64_t A382353_next = 1;   /* next block start */
static uint64_t A382353_D = 0;      /* sum of d(k) for k < next */
static uint64_t *A382353_pr = NULL;
static size_t A382353_np = 0;
static uint64_t A382353_plimit = 0;
static uint64_t A382353_s = 0;

static void A382353_primes_upto(uint64_t lim) {
    free(A382353_pr);
    char *c = (char *)calloc((size_t)lim + 1, 1);
    size_t cnt = 0;
    A382353_pr = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)lim / 2 + 2));
    for (uint64_t i = 2; i <= lim; i++) {
        if (!c[i]) {
            A382353_pr[cnt++] = i;
            for (uint64_t j = i * i; j <= lim; j += i) c[j] = 1;
        }
    }
    free(c);
    A382353_np = cnt;
    A382353_plimit = lim;
}

static uint64_t A382353_rem[A382353_B];
static uint64_t A382353_pil[A382353_B];
static uint64_t A382353_dv[A382353_B];

static void A382353_block(void) {
    uint64_t lo = A382353_next, hi = lo + A382353_B - 1;
    while ((A382353_s + 1) * (A382353_s + 1) <= hi) A382353_s++;
    if (A382353_s > A382353_plimit) {
        uint64_t nl = A382353_plimit * 2;
        if (nl < A382353_s) nl = A382353_s;
        if (nl < 1000) nl = 1000;
        A382353_primes_upto(nl);
    }
    for (uint32_t i = 0; i < A382353_B; i++) {
        A382353_rem[i] = lo + i;
        A382353_pil[i] = 1;
        A382353_dv[i] = 1;
    }
    for (size_t t = 0; t < A382353_np; t++) {
        uint64_t p = A382353_pr[t];
        if (p > A382353_s) break;
        uint64_t m = ((lo + p - 1) / p) * p;
        for (; m <= hi; m += p) {
            uint32_t idx = (uint32_t)(m - lo);
            uint64_t r = A382353_rem[idx] / p;
            uint64_t pk = 1;
            uint64_t e = 1;
            while (r % p == 0) { r /= p; e++; pk *= p; }
            A382353_rem[idx] = r;
            A382353_pil[idx] *= pk * ((e + 1) * p - e);
            A382353_dv[idx] *= (e + 1);
        }
    }
    for (uint32_t i = 0; i < A382353_B; i++) {
        uint64_t r = A382353_rem[i];
        if (r > 1) {
            A382353_pil[i] *= (2 * r - 1);
            A382353_dv[i] *= 2;
        }
        A382353_D += A382353_dv[i];
        if (A382353_D % A382353_pil[i] == 0) {
            if (A382353_cnt == A382353_cap) {
                A382353_cap = A382353_cap ? A382353_cap * 2 : 64;
                A382353_found = (uint64_t *)realloc(A382353_found, A382353_cap * sizeof(uint64_t));
            }
            A382353_found[A382353_cnt++] = lo + i;
        }
    }
    A382353_next = hi + 1;
}

seq_t a(idx_t n) {
    while (A382353_cnt < (size_t)n) A382353_block();
    return (seq_t)A382353_found[n - 1];
}
