/*
 * A381864  Numbers k in A024619 such that p^(m+1) == r (mod k) where r is
 *          also in A024619 for all p | n.
 * https://oeis.org/A381864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- k is a term if k is not a prime power and, for every
 *            prime p | k, r = p^(m+1) mod k is also not a prime power, where
 *            m = floor(log_p(k)) (so p^(m+1) is the least power of p
 *            exceeding k)
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9785 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static unsigned char *A381864_pp = NULL;
static uint64_t A381864_lim = 0;

static void A381864_build(uint64_t lim)
{
    unsigned char *c = (unsigned char *)calloc((size_t)lim + 1, 1);
    unsigned char *pp = (unsigned char *)calloc((size_t)lim + 1, 1);
    uint64_t i, q, m;
    pp[0] = 1;
    if (lim >= 1) pp[1] = 1;
    for (i = 2; i <= lim; i++) {
        if (!c[i]) {
            for (q = i; q <= lim; q *= i) {
                pp[q] = 1;
                if (q > lim / i) break;
            }
            if (i <= lim / i)
                for (m = i * i; m <= lim; m += i) c[m] = 1;
        }
    }
    free(c);
    if (A381864_pp) free(A381864_pp);
    A381864_pp = pp;
    A381864_lim = lim;
}

static bool A381864_good(uint64_t k)
{
    uint64_t pr[20];
    int w = 0, i;
    uint64_t x = k, p;
    for (p = 2; p * p <= x; p++) {
        if (x % p == 0) {
            pr[w++] = p;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) pr[w++] = x;
    if (w < 2) return false;
    for (i = 0; i < w; i++) {
        uint64_t q = pr[i];
        while (q <= k) q *= pr[i];   /* least power of p exceeding k */
        uint64_t r = q % k;
        if (A381864_pp[r]) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = NULL;
    static size_t len = 0, cap = 0;
    static uint64_t next_k = 6;

    if (n == 0) return (seq_t)0;
    if (A381864_pp == NULL) A381864_build(1 << 16);

    while (len < (size_t)n) {
        uint64_t k = next_k++;
        if (k > A381864_lim) A381864_build(A381864_lim * 2);
        if (A381864_pp[k]) continue;
        if (A381864_good(k)) {
            if (len == cap) {
                size_t nc = cap ? cap * 2 : 64;
                uint64_t *t = (uint64_t *)realloc(terms, nc * sizeof(uint64_t));
                if (!t) return (seq_t)0;
                terms = t;
                cap = nc;
            }
            terms[len++] = k;
        }
    }
    return (seq_t)terms[n - 1];
}
