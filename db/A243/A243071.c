/*
 * A243071  Permutation of nonnegative integers: a(1) = 0, a(2) = 1, a(2n) =
 *          2*a(n), a(2n+1) = 1 + 2*a(A064989(2n+1)).
 * https://oeis.org/A243071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=1, a(2n)=2*a(n), a(2n+1)=1+2*a(A064989(2n+1))
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 136, u64 n <= 312,
 *            u128 n <= 726, big any n
 * Validated: standard, pass, 75 terms through n = 983 (2026-11-07)
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

static uint64_t A243071_prev_prime(uint64_t n)
{
    uint64_t p = n - 1;
    for (;; --p) {
        if (is_prime((idx_t)p)) return p;
    }
}

/* A064989 for odd m > 1 */
static uint64_t A243071_f(uint64_t m)
{
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    size_t i;
    uint64_t res = 1;
    factor_u64(m, pr, ex, &cnt);
    for (i = 0; i < cnt; ++i) {
        uint64_t b, e;
        if (pr[i] == 2) continue;
        b = A243071_prev_prime(pr[i]);
        for (e = 0; e < ex[i]; ++e) res *= b;
    }
    return res;
}

static seq_t A243071_go(uint64_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if ((n & 1ULL) == 0) {
        unsigned k = 0;
        unsigned j;
        seq_t v;
        while ((n & 1ULL) == 0) { n >>= 1; ++k; }
        if (n == 1) v = (seq_t)1, k -= 1;
        else v = A243071_go(n);
        for (j = 0; j < k; ++j) v = v * 2;
        return v;
    }
    return (seq_t)1 + (seq_t)2 * A243071_go(A243071_f(n));
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    return A243071_go((uint64_t)n);
}
