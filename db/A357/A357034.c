/*
 * A357034  a(n) is the smallest number with exactly n divisors that are hoax
 *          numbers (A019506).
 * https://oeis.org/A357034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest m with exactly n divisors in A019506 (hoax
 *            numbers); computed by a segmented sieve
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 33 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define A357034_SEG 262144u
#define A357034_MAXN 1024u

static uint64_t *a357034_pr = NULL;
static size_t a357034_pn = 0;
static uint64_t a357034_plim = 0;

static uint64_t *a357034_hx = NULL;
static size_t a357034_hn = 0, a357034_hcap = 0;

static uint64_t a357034_found[A357034_MAXN];
static bool a357034_has[A357034_MAXN];
static uint64_t a357034_next = 1; /* next segment start */

static uint32_t a357034_ds(uint64_t x)
{
    uint32_t s = 0;
    while (x) { s += (uint32_t)(x % 10); x /= 10; }
    return s;
}

static void a357034_primes_upto(uint64_t lim)
{
    uint64_t i, j;
    unsigned char *c = (unsigned char *)calloc((size_t)lim + 1, 1);
    free(a357034_pr);
    a357034_pr = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)lim / 2 + 16));
    a357034_pn = 0;
    for (i = 2; i <= lim; i++) {
        if (!c[i]) {
            a357034_pr[a357034_pn++] = i;
            for (j = i * i; j <= lim; j += i) c[j] = 1;
        }
    }
    free(c);
    a357034_plim = lim;
}

static void a357034_segment(void)
{
    uint64_t lo = a357034_next, hi = lo + A357034_SEG; /* [lo,hi) */
    uint64_t *rem = (uint64_t *)malloc(sizeof(uint64_t) * A357034_SEG);
    uint32_t *sum = (uint32_t *)calloc(A357034_SEG, sizeof(uint32_t));
    uint16_t *cnt = (uint16_t *)calloc(A357034_SEG, sizeof(uint16_t));
    uint64_t need = 1, i;
    size_t k, hstart;

    while (need * need < hi) need++;
    if (need + 1 > a357034_plim) a357034_primes_upto((need + 1) * 2 + 100);

    for (i = 0; i < A357034_SEG; i++) rem[i] = lo + i;
    for (k = 0; k < a357034_pn; k++) {
        uint64_t p = a357034_pr[k], x;
        uint32_t d;
        if (p * p >= hi) break;
        d = a357034_ds(p);
        x = ((lo + p - 1) / p) * p;
        if (x < 2 * p) x = 2 * p;
        for (; x < hi; x += p) {
            uint64_t r = rem[x - lo];
            sum[x - lo] += d;
            do { r /= p; } while (r % p == 0);
            rem[x - lo] = r;
        }
    }
    hstart = a357034_hn;
    for (i = 0; i < A357034_SEG; i++) {
        uint64_t m = lo + i;
        uint32_t s = sum[i];
        if (s == 0) continue; /* 1 or prime */
        if (rem[i] > 1) s += a357034_ds(rem[i]);
        if (s == a357034_ds(m)) {
            if (a357034_hn == a357034_hcap) {
                a357034_hcap = a357034_hcap ? a357034_hcap * 2 : 1024;
                a357034_hx = (uint64_t *)realloc(a357034_hx, sizeof(uint64_t) * a357034_hcap);
            }
            a357034_hx[a357034_hn++] = m;
        }
    }
    (void)hstart;
    for (k = 0; k < a357034_hn; k++) {
        uint64_t h = a357034_hx[k], x;
        if (h >= hi) break;
        x = ((lo + h - 1) / h) * h;
        for (; x < hi; x += h) cnt[x - lo]++;
    }
    for (i = 0; i < A357034_SEG; i++) {
        unsigned c = cnt[i];
        if (c < A357034_MAXN && !a357034_has[c]) {
            a357034_has[c] = true;
            a357034_found[c] = lo + i;
        }
    }
    a357034_next = hi;
    free(rem);
    free(sum);
    free(cnt);
}

seq_t a(idx_t n)
{
    if (n >= A357034_MAXN) return 0;
    while (!a357034_has[n]) a357034_segment();
    return (seq_t)a357034_found[n];
}
