/*
 * A353010  a(n) = maximal d such that Product_{k=0..m} binomial(m,k) is
 *          divisible by m^(m+d), where m = A276710(n).
 * https://oeis.org/A353010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max d with m^(m+d) | Product_{k=0..m} binomial(m,k), m =
 *            A276710(n); computed as min over primes p|m of floor(E_p/v_p(m))
 *            - m, E_p = (m+1)*v_p(m!) - 2*Sum_{k=1..m} v_p(k!); m found by
 *            incremental search
 * Range:     u8 n <= 3, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 77 terms through n = 77 (2026-11-10)
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

static inline seq_t A353010_legendre(idx_t n, idx_t p) {
    seq_t res = 0;
    idx_t div = n;
    while (div) {
        div /= p;
        res += div;
    }
    return res;
}

static inline seq_t A353010_sum_floor_div(idx_t m, idx_t t) {
    idx_t L = m / t;
    idx_t r = m % t;
    seq_t sum = (seq_t)t * (seq_t)L * (seq_t)(L - 1) / 2;
    sum += (seq_t)(r + 1) * (seq_t)L;
    return sum;
}

static inline seq_t A353010_compute_Sp(idx_t m, idx_t p) {
    seq_t res = 0;
    idx_t power = p;
    while (power <= m) {
        res += A353010_sum_floor_div(m, power);
        if (power > m / p) break;
        power *= p;
    }
    return res;
}

/* For composite m returns true if m is in A276710 and sets *q = max t with m^t | product */
static bool A353010_eval(idx_t m, seq_t *q) {
    if (is_prime(m)) return false;
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(m, pr, ex, &cnt);
    bool have = false;
    seq_t best = 0;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t p = pr[i];
        seq_t V = A353010_legendre(m, p);
        seq_t S = A353010_compute_Sp(m, p);
        seq_t E = (seq_t)(m + 1) * V - (seq_t)2 * S;
        seq_t t = E / (seq_t)ex[i];
        if (!have || t < best) { best = t; have = true; }
    }
    if (best < (seq_t)(m - 1)) return false;
    *q = best;
    return true;
}

seq_t a(idx_t n) {
    static idx_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static idx_t last_checked = 1;

    if (terms_len < n) {
        idx_t m = last_checked + 1;
        while (terms_len < n) {
            seq_t q;
            if (A353010_eval(m, &q)) {
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
    idx_t mm = terms[n - 1];
    seq_t q = 0;
    A353010_eval(mm, &q);
    return q - (seq_t)mm;
}
