/*
 * A280132  Partial products of A029940 (Product_{d|n} phi(d)).
 * https://oeis.org/A280132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1..n} A029940(i) (A029940(i) = ∏_{d|i} φ(d))
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow_uint.h"

/* Factorization of n (n ≤ 10000 is assumed).  Result stored in primes[] and
   exps[], number of distinct primes returned in *count.  Arrays large
   enough for all distinct primes up to n. */
static void factorize(idx_t n, idx_t primes[], idx_t exps[], size_t *count)
{
    idx_t tmp = n;
    size_t pc = 0;
    size_t c = 0;
    /* handle factor 2 */
    while ((tmp & 1) == 0) { tmp >>= 1; c++; }
    if (c) { primes[pc] = 2; exps[pc] = c; pc++; }
    /* odd factors */
    for (idx_t f = 3; f * f <= tmp; f += 2) {
        c = 0;
        while (tmp % f == 0) { tmp /= f; c++; }
        if (c) { primes[pc] = f; exps[pc] = c; pc++; }
    }
    if (tmp > 1) { primes[pc] = tmp; exps[pc] = 1; pc++; }
    *count = pc;
}

/* Compute ∏_{d|n} φ(d) using the exact formula:
   If n = ∏ p_i^e_i, let D = ∏ (e_i+1). Then
   ∏_{d|n} φ(d) = ∏_{i} p_i^{(e_i(e_i-1)/2) * (D/(e_i+1))}
                    * (p_i-1)^{e_i * (D/(e_i+1))}. */
static seq_t phi_product(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* factor n */
    idx_t primes[64];
    idx_t exps[64];
    size_t pc = 0;
    factorize(n, primes, exps, &pc);

    /* compute D = ∏ (e_i + 1) */
    idx_t D = 1;
    for (size_t i = 0; i < pc; ++i) D *= (exps[i] + 1);

    /* compute product */
    seq_t res = 1;
    for (size_t i = 0; i < pc; ++i) {
        idx_t mult = D / (exps[i] + 1);
        idx_t exp1 = exps[i] * mult;                         /* exponent of (p-1) */
        idx_t exp2 = (exps[i] * (exps[i] - 1) / 2) * mult;   /* exponent of p   */
        seq_t t1 = pow_uint((uint64_t)(primes[i] - 1), exp1);
        seq_t t2 = pow_uint((uint64_t)primes[i], exp2);
        res *= t1;
        res *= t2;
    }
    return res;
}

/* Cache of cumulative products a(n) with indices 0..cum_limit (a(0)=1
   is unused).  Dynamic array, extended on demand. */
static seq_t *cum_cache = NULL;
static idx_t cum_limit = 0;

static void ensure_cum(idx_t n)
{
    if (cum_cache == NULL) {
        cum_cache = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        if (!cum_cache) exit(EXIT_FAILURE);
        cum_cache[0] = (seq_t)1;
        cum_limit = 0;
    }
    if (n <= cum_limit) return;
    if (cum_cache && n > cum_limit) {
        seq_t *tmp = (seq_t*)realloc(cum_cache, (n + 1) * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cum_cache = tmp;
    }
    for (idx_t i = cum_limit + 1; i <= n; ++i) {
        seq_t phi_val = phi_product(i);
        cum_cache[i] = cum_cache[i - 1] * phi_val;
    }
    cum_limit = n;
}

/* Sequence function required by the framework */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;       /* undefined for n < 1, safeguard */
    ensure_cum(n);
    return cum_cache[n];
}
