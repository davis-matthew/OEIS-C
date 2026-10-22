/*
 * A360012  a(n) is the number of triples (u,v,w) of divisors of n with u/v =
 *          v/w, and u < v < w.
 * https://oeis.org/A360012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (u,v,w) of distinct divisors of n *
 *            with u/v = v/w and u < v < w. Equivalently, * v^2 = u * w, with
 *            u, v, w divisors of n and u < v < w. * This implementation
 *            enumerates all divisors of n and tests every * possible (u,v)
 *            pair to see if w = v^2 / u is an integer divisor *...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper: compare two unsigned 64-bit values for qsort. */
static int cmp_uint64(const void *a, const void *b) {
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* Helper: factorize n into primes and exponents.
   primes and exps must have room for at least 64 entries. */
static void factorize_uint64(uint64_t n, uint64_t *primes, int *exps, size_t *cnt) {
    *cnt = 0;
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[(*cnt)] = p;
            exps[(*cnt)]   = e;
            ++(*cnt);
        }
    }
    if (tmp > 1) {
        primes[(*cnt)] = tmp;
        exps[(*cnt)]   = 1;
        ++(*cnt);
    }
}

/* Recursive generation of all divisors. */
static void gen_divisors_rec(size_t pos, uint64_t curr,
                             const uint64_t *primes, const int *exps, size_t cnt,
                             uint64_t *divs, size_t *len) {
    if (pos == cnt) {
        divs[(*len)] = curr;
        ++(*len);
        return;
    }
    uint64_t p = primes[pos];
    int e = exps[pos];
    uint64_t val = curr;
    for (int i = 0; i <= e; ++i) {
        gen_divisors_rec(pos + 1, val, primes, exps, cnt, divs, len);
        if (i < e) val *= p;
    }
}

/* Main sequence function. */
seq_t a(idx_t n) {
    /* Safe cast of n to uint64_t; idx_t is unsigned hence fits. */
    uint64_t nn = (uint64_t)n;
    if (nn == 0) return (seq_t)0;   /* never occurs for offset 1 */

    /* Factorize nn. */
    uint64_t primes[64];
    int exps[64];
    size_t pf_cnt;
    factorize_uint64(nn, primes, exps, &pf_cnt);

    /* Compute number of divisors to allocate array. */
    size_t div_count = 1;
    for (size_t i = 0; i < pf_cnt; ++i) {
        div_count *= (size_t)(exps[i] + 1);
    }

    uint64_t *divs = (uint64_t *)malloc(div_count * sizeof(uint64_t));
    if (!divs) return (seq_t)0; /* allocation failure fallback */

    size_t len = 0;
    gen_divisors_rec(0, 1ULL, primes, exps, pf_cnt, divs, &len);
    /* len should equal div_count */
    qsort(divs, len, sizeof(uint64_t), cmp_uint64);

    seq_t count = 0;
    /* For each middle divisor v */
    for (size_t vi = 0; vi < len; ++vi) {
        uint64_t v = divs[vi];
        for (size_t ui = 0; ui < vi; ++ui) {    /* u < v */
            uint64_t u = divs[ui];
            /* Ensure v^2 divisible by u. Use 128-bit to avoid overflow. */
            __uint128_t v_sq = (__uint128_t)v * (__uint128_t)v;
            if (v_sq % u != 0) continue;
            uint64_t w = (uint64_t)(v_sq / u);
            if (w <= v) continue;           /* need u < v < w */
            if (nn % w != 0) continue;       /* w must divide n */
            ++count;
        }
    }
    free(divs);
    return count;
}
