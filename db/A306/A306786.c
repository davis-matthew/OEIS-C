/*
 * A306786  Primes p such that p^2 divides A087935(p).
 * https://oeis.org/A306786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th prime p such that p^2 divides A087935(p), where
 *            A087935(n) = A087935(n-4) + A087935(n-5), A087935(0)=5,
 *            A087935(1)=A087935(2)=A087935(3)=0, A087935(4)=4; computed by a
 *            prime sieve plus x^p mod (x^5-x-1, p^2) with Montgomery
 *            arithmetic
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned __int128 pj_u128;

static uint64_t pj_m, pj_ninv;

static uint64_t pj_redc(pj_u128 T)
{
    uint64_t q = (uint64_t)T * pj_ninv;
    pj_u128 t = (T + (pj_u128)q * pj_m) >> 64;
    uint64_t r = (uint64_t)t;
    if (r >= pj_m) r -= pj_m;
    return r;
}

static void pj_square(uint64_t *a)
{
    pj_u128 c[9];
    for (int i = 0; i < 9; i++) c[i] = 0;
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            c[i + j] += (pj_u128)a[i] * a[j];
    pj_u128 d0 = c[0] + c[5];
    pj_u128 d1 = c[1] + c[5] + c[6];
    pj_u128 d2 = c[2] + c[6] + c[7];
    pj_u128 d3 = c[3] + c[7] + c[8];
    pj_u128 d4 = c[4] + c[8];
    a[0] = pj_redc(d0);
    a[1] = pj_redc(d1);
    a[2] = pj_redc(d2);
    a[3] = pj_redc(d3);
    a[4] = pj_redc(d4);
}

static uint64_t pj_addm(uint64_t a, uint64_t b)
{
    uint64_t s = a + b;
    if (s >= pj_m) s -= pj_m;
    return s;
}

/* true if p^2 | Perrin5(p), p odd prime */
static bool pj_test(uint64_t p)
{
    uint64_t m = p * p;
    pj_m = m;
    uint64_t inv = m;
    for (int i = 0; i < 6; i++) inv *= 2 - m * inv;
    pj_ninv = (uint64_t)0 - inv;
    uint64_t one = ((uint64_t)0 - m) % m; /* R mod m */
    uint64_t a[5] = {0, one, 0, 0, 0};
    int top = 63;
    while (!((p >> top) & 1)) top--;
    for (int b = top - 1; b >= 0; b--) {
        pj_square(a);
        if ((p >> b) & 1) {
            uint64_t c4 = a[4];
            uint64_t n0 = c4;
            uint64_t n1 = pj_addm(a[0], c4);
            uint64_t n2 = a[1], n3 = a[2], n4 = a[3];
            a[0] = n0; a[1] = n1; a[2] = n2; a[3] = n3; a[4] = n4;
        }
    }
    /* Perrin(p) = 5*b0 + 4*b4 (Montgomery form is irrelevant for divisibility) */
    pj_u128 P = (pj_u128)5 * a[0] + (pj_u128)4 * a[4];
    return (P % m) == 0;
}

seq_t a(idx_t n)
{
    static uint64_t found[256];
    static size_t cnt = 0;
    static uint64_t checked = 1;
    static uint64_t hi = 1 << 15;

    while (cnt < (size_t)n) {
        hi <<= 1;
        size_t half = (size_t)(hi / 2 + 1);
        unsigned char *comp = (unsigned char *)calloc(half, 1);
        if (!comp) exit(1);
        /* comp[i] represents odd number 2i+1 */
        for (uint64_t q = 3; q * q <= hi; q += 2) {
            if (comp[q >> 1]) continue;
            for (uint64_t k = q * q; k <= hi; k += 2 * q) comp[k >> 1] = 1;
        }
        for (uint64_t p = checked + 1; p <= hi; p++) {
            if (p == 2) {
                if (cnt < 256) found[cnt++] = 2;
                continue;
            }
            if ((p & 1) == 0 || p < 3 || comp[p >> 1]) continue;
            if (pj_test(p)) {
                if (cnt < 256) found[cnt++] = p;
            }
        }
        checked = hi;
        free(comp);
    }
    return (seq_t)found[(size_t)n - 1];
}
