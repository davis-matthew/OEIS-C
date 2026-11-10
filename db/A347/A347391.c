/*
 * A347391  Numbers k such that sigma(k) is either their sibling in Doudna
 *          tree (A005940), or one of the sibling's descendants.
 * https://oeis.org/A347391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   search over k >= 3. A segmented sieve gives the factorization of
 *            k and sigma(k). s = sibling of k in the Doudna tree: s =
 *            2*A064989(k) for odd k, s = A003961(k/2) for even k. k is a term
 *            iff walking up from x = sigma(k) (parent(x) = x/2 for even x,
 *            A064989(x) for odd x) reaches s exactly
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-10)
 * Helpers:   is_prime_u64, isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_u64.h"
#include "../utils/isqrt_u64.h"

#define A347391_B 65536u

static uint8_t *A347391_bits = NULL;
static uint64_t A347391_lim = 0;
static uint64_t *A347391_sp = NULL, *A347391_nx = NULL, *A347391_pv = NULL;
static size_t A347391_nsp = 0;

static bool A347391_isp(uint64_t v) {
    if (v < 2) return false;
    if (v <= A347391_lim) return (A347391_bits[v >> 3] >> (v & 7)) & 1;
    return is_prime_u64(v);
}
static uint64_t A347391_nextp(uint64_t p) {
    p++;
    while (!A347391_isp(p)) p++;
    return p;
}
static uint64_t A347391_prevp(uint64_t p) {
    if (p <= 2) return 1;
    p--;
    while (!A347391_isp(p)) p--;
    return p;
}

static void A347391_ensure(uint64_t need) {
    if (need <= A347391_lim) return;
    uint64_t nl = need + need / 2;
    if (nl < 2 * A347391_lim) nl = 2 * A347391_lim;
    if (nl < (1u << 20)) nl = 1u << 20;
    free(A347391_bits);
    size_t nb = (size_t)(nl >> 3) + 1;
    A347391_bits = (uint8_t *)malloc(nb);
    memset(A347391_bits, 0xFF, nb);
    A347391_bits[0] &= (uint8_t)~3u;
    for (uint64_t i = 2; i * i <= nl; i++) {
        if ((A347391_bits[i >> 3] >> (i & 7)) & 1) {
            for (uint64_t j = i * i; j <= nl; j += i)
                A347391_bits[j >> 3] &= (uint8_t)~(1u << (j & 7));
        }
    }
    A347391_lim = nl;
    uint64_t sq = isqrt_u64(nl) + 2;
    free(A347391_sp); free(A347391_nx); free(A347391_pv);
    size_t cnt = 0;
    for (uint64_t i = 2; i <= sq; i++) if (A347391_isp(i)) cnt++;
    A347391_sp = (uint64_t *)malloc((cnt + 1) * sizeof(uint64_t));
    A347391_nx = (uint64_t *)malloc((cnt + 1) * sizeof(uint64_t));
    A347391_pv = (uint64_t *)malloc((cnt + 1) * sizeof(uint64_t));
    size_t c = 0;
    for (uint64_t i = 2; i <= sq; i++) {
        if (A347391_isp(i)) {
            A347391_sp[c] = i;
            A347391_nx[c] = A347391_nextp(i);
            A347391_pv[c] = A347391_prevp(i);
            c++;
        }
    }
    A347391_nsp = cnt;
}

/* A064989 for odd y */
static uint64_t A347391_a064989(uint64_t y) {
    uint64_t res = 1;
    for (uint64_t d = 3; d * d <= y; d += 2) {
        if (y % d == 0) {
            uint64_t pv = A347391_prevp(d);
            while (y % d == 0) { y /= d; res *= pv; }
        }
    }
    if (y > 1) res *= A347391_prevp(y);
    return res;
}

static bool A347391_desc(uint64_t x, uint64_t s) {
    while (x > s) {
        if ((x & 1) == 0) x >>= 1;
        else x = A347391_a064989(x);
    }
    return x == s;
}

static uint64_t *A347391_terms = NULL;
static size_t A347391_cnt = 0, A347391_cap = 0;
static uint64_t A347391_nextL = 1;

static void A347391_block(void) {
    static uint64_t rem[A347391_B], sg[A347391_B], pa[A347391_B], lp[A347391_B], bm[A347391_B];
    uint64_t L = A347391_nextL, hi = L + A347391_B - 1;
    A347391_nextL = hi + 1;
    A347391_ensure(hi + 4096);
    uint64_t sq = isqrt_u64(hi);
    for (uint32_t i = 0; i < A347391_B; i++) {
        rem[i] = L + i; sg[i] = 1; pa[i] = 1; lp[i] = 1; bm[i] = 0;
    }
    for (size_t j = 0; j < A347391_nsp; j++) {
        uint64_t p = A347391_sp[j];
        if (p > sq) break;
        uint64_t t = ((L + p - 1) / p) * p;
        for (; t <= hi; t += p) {
            uint32_t i = (uint32_t)(t - L);
            uint64_t e = 0;
            while (rem[i] % p == 0) { rem[i] /= p; e++; }
            uint64_t term = 1, pw = 1;
            for (uint64_t u = 0; u < e; u++) { pw *= p; term += pw; }
            sg[i] *= term;
            uint64_t base = (t & 1) ? A347391_pv[j] : A347391_nx[j];
            for (uint64_t u = 0; u < e; u++) pa[i] *= base;
            lp[i] = p;
            uint64_t bd = (e == 1) ? (p == 2 ? 3 : (p + 1) / 2) : term;
            if (bd > bm[i]) bm[i] = bd;
        }
    }
    for (uint32_t i = 0; i < A347391_B; i++) {
        uint64_t k = L + i;
        if (k < 3) continue;
        uint64_t q = rem[i];
        uint64_t P = (q > 1) ? q : lp[i];
        uint64_t x = sg[i] * (q > 1 ? q + 1 : 1);
        uint64_t s, G;
        if (k & 1) {
            uint64_t pq = (q > 1) ? A347391_prevp(q) : 1;
            s = 2 * pa[i] * pq;
            G = (q > 1) ? pq : A347391_prevp(P);
        } else {
            uint64_t nq = (q > 1) ? A347391_nextp(q) : 1;
            s = (pa[i] / 3) * nq;
            G = (q > 1) ? nq : A347391_nextp(P);
        }
        uint64_t Bd = bm[i];
        if (q > 1 && (q + 1) / 2 > Bd) Bd = (q + 1) / 2;
        while (x > s && (x & 1) == 0) x >>= 1;
        bool ok;
        if (x == s) ok = true;
        else if (x < s) ok = false;
        else if (Bd <= G) ok = false; /* odd x > s needs a prime factor > G */
        else ok = A347391_desc(x, s);
        if (ok) {
            if (A347391_cnt == A347391_cap) {
                A347391_cap = A347391_cap ? A347391_cap * 2 : 64;
                A347391_terms = (uint64_t *)realloc(A347391_terms, A347391_cap * sizeof(uint64_t));
            }
            A347391_terms[A347391_cnt++] = k;
        }
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    while (A347391_cnt < (size_t)n) A347391_block();
    return (seq_t)A347391_terms[n - 1];
}
