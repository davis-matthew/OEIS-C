/*
 * A324790  Indices of zeros in A022837.
 * https://oeis.org/A324790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = index k of the n-th zero of A022837; found by walking
 *            A022837 (a(0)=1, c(i)=+1 if prev<p(i) else -1) over primes from
 *            a segmented sieve
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A324790_SEG (1u << 20)

static uint64_t A324790_found[64];
static size_t   A324790_nfound = 0;
static bool     A324790_init = false;
static uint32_t *A324790_base = NULL;
static size_t   A324790_nbase = 0;
static unsigned char *A324790_buf = NULL;
static uint64_t A324790_lo = 2;
static uint64_t A324790_cur = 1;   /* A022837 value at index idx */
static uint64_t A324790_idx = 0;

static void A324790_setup(void)
{
    unsigned char *s = (unsigned char *)calloc(A324790_SEG + 1, 1);
    A324790_base = (uint32_t *)malloc(90000 * sizeof(uint32_t));
    A324790_buf = (unsigned char *)malloc(A324790_SEG);
    if (!s || !A324790_base || !A324790_buf) abort();
    for (uint64_t i = 2; i <= A324790_SEG; i++) {
        if (!s[i]) {
            if (A324790_nbase < 90000) A324790_base[A324790_nbase++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= A324790_SEG; j += i) s[j] = 1;
        }
    }
    free(s);
    A324790_init = true;
}

static void A324790_segment(void)
{
    uint64_t lo = A324790_lo, hi = lo + A324790_SEG;
    for (uint32_t i = 0; i < A324790_SEG; i++) A324790_buf[i] = 1;
    for (size_t k = 0; k < A324790_nbase; k++) {
        uint64_t p = A324790_base[k];
        if (p * p >= hi) break;
        uint64_t st = (lo + p - 1) / p * p;
        if (st < p * p) st = p * p;
        for (uint64_t j = st; j < hi; j += p) A324790_buf[j - lo] = 0;
    }
    for (uint32_t i = 0; i < A324790_SEG; i++) {
        if (!A324790_buf[i]) continue;
        uint64_t p = lo + i;
        A324790_idx++;
        if (A324790_cur < p) A324790_cur += p;
        else {
            A324790_cur -= p;
            if (A324790_cur == 0 && A324790_nfound < 64)
                A324790_found[A324790_nfound++] = A324790_idx;
        }
    }
    A324790_lo = hi;
}

seq_t a(idx_t n)
{
    if (!A324790_init) A324790_setup();
    if (n > 64) n = 64;
    while (A324790_nfound < (size_t)n)
        A324790_segment();
    return (seq_t)A324790_found[n - 1];
}
