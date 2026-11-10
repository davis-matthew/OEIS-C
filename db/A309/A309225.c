/*
 * A309225  Indices m such that A008348(m)=0.
 * https://oeis.org/A309225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1)=0; for n>=2, a(n) = n-th index m>=1 with A008348(m)=0,
 *            where A008348(0)=0, A008348(m)=A008348(m-1)+prime(m) if
 *            A008348(m-1)<prime(m), else A008348(m-1)-prime(m). Computed by a
 *            search with an incremental segmented prime sieve
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define A309225_SEG 131072u
#define A309225_BASE_LIM 65536u
#define A309225_MAXT 64

static uint32_t A309225_base[7000];
static int A309225_nbase = 0;
static uint8_t *A309225_flags = NULL;
static uint32_t *A309225_buf = NULL;
static size_t A309225_buf_n = 0, A309225_buf_i = 0;
static uint64_t A309225_lo = 3;
static bool A309225_two_done = false;

static uint64_t A309225_found[A309225_MAXT];
static int A309225_nfound = 0;
static uint64_t A309225_m = 0;
static uint64_t A309225_v = 0;
static bool A309225_init = false;

static void A309225_setup(void)
{
    uint32_t i, j;
    uint8_t *s = (uint8_t *)calloc(A309225_BASE_LIM + 1, 1);
    if (!s) abort();
    for (i = 3; i <= A309225_BASE_LIM; i += 2) {
        if (!s[i]) {
            A309225_base[A309225_nbase++] = i;
            for (j = i * i; j <= A309225_BASE_LIM; j += 2 * i) s[j] = 1;
        }
    }
    free(s);
    A309225_flags = (uint8_t *)malloc(A309225_SEG);
    A309225_buf = (uint32_t *)malloc(sizeof(uint32_t) * A309225_SEG);
    if (!A309225_flags || !A309225_buf) abort();
}

static void A309225_fill(void)
{
    uint64_t lo = A309225_lo;
    uint64_t hi = lo + 2ull * A309225_SEG;
    int k;
    uint32_t i;
    for (i = 0; i < A309225_SEG; i++) A309225_flags[i] = 0;
    for (k = 0; k < A309225_nbase; k++) {
        uint64_t p = A309225_base[k];
        uint64_t first = p * p;
        if (first >= hi) break;
        if (first < lo) {
            first = ((lo + p - 1) / p) * p;
            if ((first & 1) == 0) first += p;
        }
        for (uint64_t x = first; x < hi; x += 2 * p)
            A309225_flags[(x - lo) >> 1] = 1;
    }
    A309225_buf_n = 0;
    A309225_buf_i = 0;
    for (i = 0; i < A309225_SEG; i++)
        if (!A309225_flags[i])
            A309225_buf[A309225_buf_n++] = (uint32_t)(lo + 2ull * i);
    A309225_lo = hi;
}

static uint64_t A309225_next_prime(void)
{
    if (!A309225_two_done) { A309225_two_done = true; return 2; }
    while (A309225_buf_i >= A309225_buf_n) A309225_fill();
    return A309225_buf[A309225_buf_i++];
}

seq_t a(idx_t n)
{
    if (!A309225_init) {
        A309225_init = true;
        A309225_setup();
        A309225_found[A309225_nfound++] = 0; /* index m = 0 */
    }
    while ((uint64_t)A309225_nfound < (uint64_t)n && A309225_nfound < A309225_MAXT) {
        uint64_t p = A309225_next_prime();
        A309225_m++;
        if (A309225_v < p) A309225_v += p; else A309225_v -= p;
        if (A309225_v == 0) A309225_found[A309225_nfound++] = A309225_m;
    }
    return (seq_t)A309225_found[n - 1];
}
