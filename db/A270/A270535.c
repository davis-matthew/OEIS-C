/*
 * A270535  Integers k such that A001359(k) + A001359(k+3) = A001359(k+1) +
 *          A001359(k+2).
 * https://oeis.org/A270535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: segmented sieve of Eratosthenes enumerates the lesser
 *            twin primes t(1)=3, t(2)=5, ...; a(n) = n-th k with t(k)+t(k+3)
 *            = t(k+1)+t(k+2)
 * Range:     u8 n <= 20, u16 n <= 1360, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9821 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define A270535_SEG 262144u  /* odd numbers per segment */

static uint64_t *a270535_ks = NULL;
static size_t a270535_cnt = 0, a270535_cap = 0;

static uint32_t *a270535_bp = NULL;   /* odd base primes */
static size_t a270535_bpc = 0;
static uint64_t a270535_bplim = 0;

static uint64_t a270535_lo = 3;       /* next odd number to sieve */
static uint64_t a270535_lastp = 0;    /* last prime seen */
static uint64_t a270535_w[4];
static uint64_t a270535_tc = 0;       /* twin pairs seen */
static unsigned char *a270535_seg = NULL;

static void a270535_base(uint64_t need) {
    uint64_t lim, i, j;
    unsigned char *c;
    if (need <= a270535_bplim) return;
    lim = a270535_bplim ? a270535_bplim : 1024;
    while (lim < need) lim *= 2;
    c = (unsigned char *)malloc((size_t)(lim + 1));
    memset(c, 0, (size_t)(lim + 1));
    for (i = 2; i * i <= lim; i++)
        if (!c[i])
            for (j = i * i; j <= lim; j += i) c[j] = 1;
    free(a270535_bp);
    a270535_bp = (uint32_t *)malloc((size_t)(lim + 1) * sizeof(uint32_t) / 2 + 16);
    a270535_bpc = 0;
    for (i = 3; i <= lim; i += 2)
        if (!c[i]) a270535_bp[a270535_bpc++] = (uint32_t)i;
    free(c);
    a270535_bplim = lim;
}

static void a270535_twin(uint64_t t) {
    a270535_w[0] = a270535_w[1];
    a270535_w[1] = a270535_w[2];
    a270535_w[2] = a270535_w[3];
    a270535_w[3] = t;
    a270535_tc++;
    if (a270535_tc >= 4 &&
        a270535_w[0] + a270535_w[3] == a270535_w[1] + a270535_w[2]) {
        if (a270535_cnt == a270535_cap) {
            a270535_cap = a270535_cap ? a270535_cap * 2 : 1024;
            a270535_ks = (uint64_t *)realloc(a270535_ks, a270535_cap * sizeof(uint64_t));
        }
        a270535_ks[a270535_cnt++] = a270535_tc - 3;
    }
}

static void a270535_segment(void) {
    uint64_t lo = a270535_lo;
    uint64_t hi = lo + 2 * (uint64_t)A270535_SEG; /* exclusive */
    uint64_t r, i;
    size_t b;
    unsigned char *s;
    if (!a270535_seg) a270535_seg = (unsigned char *)malloc(A270535_SEG);
    s = a270535_seg;
    memset(s, 0, A270535_SEG);
    r = 1;
    while ((r + 1) * (r + 1) < hi) r++;
    a270535_base(r + 1);
    for (b = 0; b < a270535_bpc; b++) {
        uint64_t q = a270535_bp[b], st;
        if (q * q >= hi) break;
        st = q * q;
        if (st < lo) {
            st = ((lo + q - 1) / q) * q;
            if (!(st & 1)) st += q;
        }
        for (i = (st - lo) / 2; i < A270535_SEG; i += q) s[i] = 1;
    }
    for (i = 0; i < A270535_SEG; i++) {
        if (!s[i]) {
            uint64_t p = lo + 2 * i;
            if (a270535_lastp && p - a270535_lastp == 2) a270535_twin(a270535_lastp);
            a270535_lastp = p;
        }
    }
    a270535_lo = hi;
}

seq_t a(idx_t n) {
    while ((uint64_t)a270535_cnt < (uint64_t)n) a270535_segment();
    return (seq_t)a270535_ks[n - 1];
}
