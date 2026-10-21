/*
 * A276710  Composite numbers m such that Product_{k=0..m} binomial(m,k) is
 *          divisible by m^(m-1).
 * https://oeis.org/A276710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1540, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for composite m such that
 *   Π_{k=0..m} binomial(m,k) is divisible by m^(m-1)  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"          /* bool is_prime(idx_t) */
#include "../utils/factor_u64.h"        /* void factor_u64(uint64_t,uint64_t*,uint64_t*,size_t*) */

/* Legendre exponent of prime p in n! */
static inline seq_t legendre(idx_t n, idx_t p) {
    seq_t res = 0;
    idx_t div = n;
    while (div) {
        div /= p;
        res += div;
    }
    return res;
}

/* Sum_{k=1..m} floor(k / t) */
static inline seq_t sum_floor_div(idx_t m, idx_t t) {
    idx_t L = m / t;
    idx_t r = m % t;
    seq_t sum = (seq_t)t * (seq_t)L * (seq_t)(L - 1) / 2;
    sum += (seq_t)(r + 1) * (seq_t)L;
    return sum;
}

/* S_p(m) = Σ_{k=1..m} v_p(k!) */
static inline seq_t compute_Sp(idx_t m, idx_t p) {
    seq_t res = 0;
    idx_t power = p;
    while (power <= m) {
        res += sum_floor_div(m, power);
        if (power > m / p) break;   /* avoid overflow */
        power *= p;
    }
    return res;
}

/* Test whether m satisfies the divisibility condition */
static bool test_m(idx_t m) {
    if (is_prime(m)) return false;   /* primes are excluded */

    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(m, pr, ex, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        idx_t p = pr[i];
        seq_t V = legendre(m, p);
        seq_t S = compute_Sp(m, p);
        seq_t E = (seq_t)(m + 1) * V - (seq_t)2 * S;
        seq_t needed = (seq_t)(m - 1) * (seq_t)ex[i];
        if (E < needed) return false;
    }
    return true;
}

/* Return the n-th term of A276710 (1‑based index) */
seq_t a(idx_t n) {
    static idx_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static idx_t last_checked = 1;

    if (terms_len < n) {
        idx_t m = last_checked + 1;
        while (terms_len < n) {
            if (test_m(m)) {
                if (terms_len + 1 > terms_cap) {
                    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                    terms = (idx_t*)realloc(terms, new_cap * sizeof(idx_t));
                    terms_cap = new_cap;
                }
                terms[terms_len++] = m;
            }
            ++m;
        }
        last_checked = m - 1;
    }
    return (seq_t)terms[n - 1];
}
