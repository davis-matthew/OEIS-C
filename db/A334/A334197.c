/*
 * A334197  Positions of records in A334090; After 1, positions of record
 *          jumps upward in A064097.
 * https://oeis.org/A334197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = position of n-th record in A334090, where A334090(1)=0
 *            and A334090(m) = A064097(m)-A064097(m-1) (record upward jumps);
 *            computed by a segmented sieve scan of A064097
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define A334197_SEG   (1u << 18)
#define A334197_PLIM  65536u
#define A334197_MAXR  64

static uint8_t  *A334197_arr = NULL;     /* A064097 values */
static uint64_t  A334197_cap = 0;
static uint64_t  A334197_next = 2;       /* next index to compute */
static uint32_t *A334197_primes = NULL;
static int       A334197_np = 0;
static uint64_t  A334197_rec[A334197_MAXR];
static int       A334197_nrec = 0;
static int       A334197_best = 0;

static void A334197_init(void)
{
    if (A334197_primes) return;
    char *s = (char *)calloc(A334197_PLIM + 1, 1);
    A334197_primes = (uint32_t *)malloc(7000 * sizeof(uint32_t));
    for (uint32_t i = 2; i <= A334197_PLIM; i++) {
        if (!s[i]) {
            A334197_primes[A334197_np++] = i;
            for (uint64_t j = (uint64_t)i * i; j <= A334197_PLIM; j += i) s[j] = 1;
        }
    }
    free(s);
    A334197_cap = 1u << 20;
    A334197_arr = (uint8_t *)calloc(A334197_cap, 1);
    A334197_rec[0] = 1;
    A334197_nrec = 1;
    A334197_best = 0;
}

static void A334197_segment(void)
{
    uint64_t lo = A334197_next;
    uint64_t hi = lo + A334197_SEG;
    if (hi > A334197_cap) {
        uint64_t nc = A334197_cap;
        while (nc < hi) nc <<= 1;
        A334197_arr = (uint8_t *)realloc(A334197_arr, nc);
        memset(A334197_arr + A334197_cap, 0, nc - A334197_cap);
        A334197_cap = nc;
    }
    static uint32_t spf[A334197_SEG];
    memset(spf, 0, sizeof(spf));
    for (int k = 0; k < A334197_np; k++) {
        uint64_t q = A334197_primes[k];
        if (q * q >= hi) break;
        uint64_t st = ((lo + q - 1) / q) * q;
        if (st < q * q) st = q * q;
        for (uint64_t j = st; j < hi; j += q)
            if (!spf[j - lo]) spf[j - lo] = (uint32_t)q;
    }
    uint8_t *a = A334197_arr;
    a[1] = 0;
    for (uint64_t n = lo; n < hi; n++) {
        uint32_t q = spf[n - lo];
        if (q == 0) a[n] = (uint8_t)(1 + a[n - 1]);
        else a[n] = (uint8_t)(a[q] + a[n / q]);
        int d = (int)a[n] - (int)a[n - 1];
        if (d > A334197_best) {
            A334197_best = d;
            if (A334197_nrec < A334197_MAXR) A334197_rec[A334197_nrec++] = n;
        }
    }
    A334197_next = hi;
}

seq_t a(idx_t n)
{
    A334197_init();
    while ((uint64_t)A334197_nrec < (uint64_t)n && A334197_nrec < A334197_MAXR)
        A334197_segment();
    return (seq_t)A334197_rec[n - 1];
}
