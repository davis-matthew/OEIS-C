/*
 * A358273  Number of binary digits of A007442(n).
 * https://oeis.org/A358273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = A070939(|A007442(n)|), where A007442(n) = Sum_{k=0..n-1}
 *            (-1)^(n-k-1) binomial(n-1,k) prime(k+1), the (n-1)-st difference
 *            of the first n primes. It is computed with an incrementally
 *            updated difference-table diagonal held in internal multi-word
 *            two's-complement integers, so no intermediate...
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 81 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_u64.h"

static uint32_t *dg7442 = NULL;     /* row i: diag[i], L limbs, two's complement */
static unsigned *res7442 = NULL;    /* res7442[m] = a(m+1) */
static size_t cap7442 = 0, L7442 = 0, cnt7442 = 0;
static uint64_t lastp7442 = 1;
static uint32_t *bufA7442 = NULL, *bufB7442 = NULL;

static uint64_t next_prime7442(uint64_t p) {
    uint64_t q = p + 1;
    while (!is_prime_u64(q)) q++;
    return q;
}

static void reshape7442(size_t newcap, size_t newL) {
    uint32_t *nd = (uint32_t *)calloc(newcap * newL, sizeof(uint32_t));
    unsigned *nr = (unsigned *)calloc(newcap, sizeof(unsigned));
    size_t i, k;
    for (i = 0; i < cnt7442; i++) {
        uint32_t *src = dg7442 + i * L7442;
        uint32_t *dst = nd + i * newL;
        uint32_t ext = (src[L7442 - 1] & 0x80000000u) ? 0xFFFFFFFFu : 0u;
        for (k = 0; k < L7442; k++) dst[k] = src[k];
        for (; k < newL; k++) dst[k] = ext;
        nr[i] = res7442[i];
    }
    free(dg7442);
    free(res7442);
    free(bufA7442);
    free(bufB7442);
    dg7442 = nd;
    res7442 = nr;
    bufA7442 = (uint32_t *)calloc(newL, sizeof(uint32_t));
    bufB7442 = (uint32_t *)calloc(newL, sizeof(uint32_t));
    cap7442 = newcap;
    L7442 = newL;
}

/* r = x - y, all L limbs (r may alias x) */
static void sub7442(uint32_t *r, const uint32_t *x, const uint32_t *y, size_t L) {
    uint64_t borrow = 0;
    size_t k;
    for (k = 0; k < L; k++) {
        uint64_t d = (uint64_t)x[k] - (uint64_t)y[k] - borrow;
        r[k] = (uint32_t)d;
        borrow = (d >> 63) & 1u;
    }
}

static unsigned bitlen_abs7442(const uint32_t *x, size_t L, uint32_t *tmp) {
    size_t k;
    const uint32_t *v = x;
    if (x[L - 1] & 0x80000000u) {
        uint64_t carry = 1;
        for (k = 0; k < L; k++) {
            uint64_t s = (uint64_t)(uint32_t)~x[k] + carry;
            tmp[k] = (uint32_t)s;
            carry = s >> 32;
        }
        v = tmp;
    }
    k = L;
    while (k > 0 && v[k - 1] == 0) k--;
    if (k == 0) return 1;
    {
        uint32_t t = v[k - 1];
        unsigned b = 0;
        while (t) { b++; t >>= 1; }
        return (unsigned)(32 * (k - 1)) + b;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    while ((idx_t)cnt7442 < n) {
        size_t needL = (cnt7442 + 72) / 32 + 2;
        if (cnt7442 >= cap7442 || needL > L7442) {
            size_t nc = cap7442, nl = L7442;
            if (cnt7442 >= nc) nc = nc ? 2 * nc : 64;
            if (needL > nl) nl = (nl * 2 > needL) ? nl * 2 : needL;
            reshape7442(nc, nl);
        }
        {
            size_t L = L7442, j, k;
            uint32_t *prev = bufA7442, *tmp = bufB7442, *sw;
            uint32_t *row0 = dg7442;
            lastp7442 = next_prime7442(lastp7442);
            memcpy(prev, row0, L * sizeof(uint32_t));
            for (k = 0; k < L; k++) row0[k] = 0;
            row0[0] = (uint32_t)lastp7442;
            row0[1] = (uint32_t)(lastp7442 >> 32);
            for (j = 1; j <= cnt7442; j++) {
                uint32_t *rj = dg7442 + j * L;
                memcpy(tmp, rj, L * sizeof(uint32_t));
                sub7442(rj, dg7442 + (j - 1) * L, prev, L);
                sw = prev; prev = tmp; tmp = sw;
            }
            res7442[cnt7442] = bitlen_abs7442(dg7442 + cnt7442 * L, L, tmp);
            cnt7442++;
        }
    }
    return (seq_t)res7442[n - 1];
}
