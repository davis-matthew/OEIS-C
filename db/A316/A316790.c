/*
 * A316790  Number of orderless same-tree-factorizations of n.
 * https://oeis.org/A316790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{n = x^y, y > 1} binomial(a(x)+y-1, y)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
/* helper: factorization of n as prime powers */
#include "../utils/factor_u64.h"

/* Euclidean gcd for unsigned integers */
static idx_t gcd_u64(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* binomial coefficient for arbitrary‑precision seq_t */
static seq_t binom_seq(seq_t n, idx_t k) {
    if (k == 0) return 1;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = n - (seq_t)(k - i);   /* n - (k-i) = n - k + i */
        res = res * num;
        res = res / i;                   /* exact division */
    }
    return res;
}

/* recursive evaluation of a(n) */
static seq_t get_a(idx_t n) {
    if (n == 1) return 1;   /* a(1) = 1 */

    /* factor n into primes and exponents */
    uint64_t primes[32], exps[32];
    size_t pcnt;
    factor_u64(n, primes, exps, &pcnt);

    /* gcd of all exponents */
    idx_t g = exps[0];
    for (size_t i = 1; i < pcnt; ++i) g = gcd_u64(g, exps[i]);

    if (g <= 1) return 1;   /* no non‑trivial perfect powers */

    seq_t sum = 0;
    for (idx_t y = 2; y <= g; ++y) {
        if (g % y != 0) continue;   /* y must divide every exponent */

        /* compute x = n^(1/y) using the exponents */
        uint64_t x = 1;
        for (size_t i = 0; i < pcnt; ++i) {
            uint64_t e = exps[i] / y;   /* integer division guaranteed */
            for (uint64_t j = 0; j < e; ++j) x *= primes[i];
        }

        seq_t ax   = get_a(x);
        seq_t term = binom_seq(ax + (seq_t)(y - 1), y);
        sum += term;
    }

    return sum + 1; /* include the trivial factorisation n itself */
}

/* public entry point */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined, but harmless */
    return get_a(n);
}
