/*
 * A245414  Number of endofunctions on [n] such that no element has a preimage
 *          of cardinality ten.
 * https://oeis.org/A245414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{⌊n/10⌋} (-1)^k · C(n,k) · n! / ((n-10k)! ·
 *            (10!)^k) · (n-k)^(n-10k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 200 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Compute binomial coefficient C(a,b) using multiplicative formula. */
static seq_t binom(idx_t a, idx_t b) {
    if (b > a) return 0;
    if (b > a - b) b = a - b;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= b; ++i) {
        res *= (seq_t)(a - b + i);
        res /= (seq_t)i;
    }
    return res;
}

/* fast exponentiation by squaring: base^exp in seq_t */
static seq_t seq_pow(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0)
        return 1;

    /* ---------- Pre‑compute factorials up to n ---------- */
    static seq_t *fac = NULL;
    static idx_t fac_cap = 0;
    if (!fac || fac_cap <= n) {
        mem_free(fac);
        fac_cap = n + 1;
        fac = mem_new(seq_t, fac_cap);
        fac[0] = 1;
        for (idx_t i = 1; i <= n; ++i)
            fac[i] = fac[i - 1] * (seq_t)i;
    }

    /* 10!  (computed once per call; inexpensive) */
    seq_t tenfact = 1;
    for (idx_t i = 1; i <= 10; ++i)
        tenfact *= (seq_t)i;

    idx_t maxk = n / 10;
    seq_t result = 0;
    seq_t pow10fact = 1;          /* (10!)^k, initialized for k = 0 */

    for (idx_t k = 0; k <= maxk; ++k) {
        /* C(n,k) */
        seq_t comb = binom(n, k);

        /* term = comb * n! / (n-10k)! */
        seq_t term = comb * fac[n];
        term = term / fac[n - 10 * k];

        /* divide by (10!)^k */
        if (k > 0)
            pow10fact *= tenfact;      /* pow10fact now = (10!)^k */
        term = term / pow10fact;

        /* multiply by (n-k)^(n-10k) */
        seq_t pow_part = seq_pow((seq_t)(n - k), (idx_t)(n - 10 * k));
        term = term * pow_part;

        /* add or subtract depending on parity of k */
        if (k & 1)
            result = result - term;
        else
            result = result + term;
    }

    return result;
}
