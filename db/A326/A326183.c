/*
 * A326183  Numbers n for which A003961(n) <= A064989(sigma(A003961(n))), but
 *          which are not squares.
 * https://oeis.org/A326183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th non-square k with A003961(k) <=
 *            A064989(sigma(A003961(k))); search restricted to the observed
 *            form k = 360000*p or 29160000*p (p prime), per the OEIS comment
 *            on A052126(a(n))
 * Range:     u8 none, u16 none, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-08)
 * Helpers:   factor_u64, is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/next_prime.h"
#include "../utils/factor_u64.h"

typedef unsigned __int128 A326183_u128;

static uint64_t A326183_prev_prime(uint64_t p) {
    uint64_t q = p - 1;
    while (q > 2 && !is_prime_u64(q)) --q;
    return q;
}

static bool A326183_test(uint64_t n) {
    uint64_t pr[64], ex[64];
    size_t cnt = 0, i;
    factor_u64(n, pr, ex, &cnt);
    uint64_t s = 1;
    for (i = 0; i < cnt; i++) {
        uint64_t q = next_prime(pr[i]);
        for (uint64_t e = 0; e < ex[i]; e++) s *= q;
    }
    /* sigma(s) */
    cnt = 0;
    factor_u64(s, pr, ex, &cnt);
    A326183_u128 sg = 1;
    for (i = 0; i < cnt; i++) {
        A326183_u128 term = 1, pw = 1;
        for (uint64_t e = 0; e < ex[i]; e++) {
            pw *= pr[i];
            term += pw;
        }
        sg *= term;
    }
    /* A064989(sg): sg assumed to fit in 64 bits */
    uint64_t m = (uint64_t)sg;
    cnt = 0;
    factor_u64(m, pr, ex, &cnt);
    A326183_u128 r = 1;
    for (i = 0; i < cnt; i++) {
        if (pr[i] == 2) continue;
        uint64_t q = A326183_prev_prime(pr[i]);
        for (uint64_t e = 0; e < ex[i]; e++) r *= q;
    }
    return (A326183_u128)s <= r;
}

static uint64_t *A326183_terms = 0;
static size_t A326183_cnt = 0, A326183_cap = 0;
static uint64_t A326183_q = 1;

seq_t a(idx_t n) {
    while (A326183_cnt < (size_t)n) {
        uint64_t q = ++A326183_q;
        uint64_t cand = 0;
        if (is_prime_u64(q)) {
            if (A326183_test(360000ULL * q)) cand = 360000ULL * q;
        } else if (q % 81 == 0 && q / 81 >= 7 && is_prime_u64(q / 81)) {
            if (A326183_test(360000ULL * q)) cand = 360000ULL * q;
        }
        if (cand) {
            if (A326183_cnt == A326183_cap) {
                A326183_cap = A326183_cap ? A326183_cap * 2 : 64;
                A326183_terms = (uint64_t *)realloc(A326183_terms, A326183_cap * sizeof(uint64_t));
            }
            A326183_terms[A326183_cnt++] = cand;
        }
    }
    return (seq_t)A326183_terms[n - 1];
}
