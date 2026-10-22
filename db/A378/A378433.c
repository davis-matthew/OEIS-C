/*
 * A378433  Dirichlet inverse of A325973, where A325973 is the arithmetic mean
 *          of {sum of unitary divisors} and {sum of squarefree divisors}.
 * https://oeis.org/A378433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -Sum_{d|n, d < n} A325973(n/d) * a(d) with a(1) = 1.
 *            A325973(m) = (∏_{p^e||m} (1+p^e) + ∏_{p||m} (1+p)) / 2 This
 *            implementation uses memoisation to handle any order of calls
 * Range:     i8 n <= 65, i16 n <= 8639, i32 n <= 8639, i64 n <= 8639,
 *            i128 n <= 8639, big any n
 * Helpers:   gen_divs_recursive
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gen_divs_recursive.h"

/* ------------------------------------------------------------------ */
/* Factorisation helpers (trial division – fast enough for n ≤ 10000) */
static void
factor(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[c]   = p;
            ex[c++] = e;
        }
    }
    if (n > 1) {
        pr[c]   = n;
        ex[c++] = 1;
    }
    *cnt = c;
}

/* Compute A325973(n) = (Π(1+p^e) + Π(1+p)) / 2. */
static seq_t
compute_f(uint64_t n)
{
    if (n == 1)
        return (seq_t)1;

    uint64_t pr[64], ex[64];
    size_t pcnt;
    factor(n, pr, ex, &pcnt);

    seq_t prod1 = 1;
    seq_t prod2 = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        seq_t p_pow = 1;
        for (uint64_t j = 0; j < e; ++j)
            p_pow *= (seq_t)p;
        prod1 *= ((seq_t)1 + p_pow);
        prod2 *= ((seq_t)1 + (seq_t)p);
    }
    return (prod1 + prod2) / 2;
}

/* ------------------------------------------------------------------ */
/* Main sequence function with static memoisation. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;              /* undefined for n < 1 */

    /* Static memoisation tables */
    static seq_t *a_cache = NULL;
    static unsigned char *a_used = NULL;
    static size_t a_cap = 0;
    static seq_t *f_cache = NULL;
    static unsigned char *f_used = NULL;
    static size_t f_cap = 0;

    /* Ensure capacity for n */
    if ((size_t)n >= a_cap) {
        size_t old_a_cap = a_cap;
        size_t old_f_cap = f_cap;
        size_t new_cap   = ((size_t)n + 1) * 2;

        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_used  = (unsigned char *)realloc(a_used,  new_cap * sizeof(unsigned char));
        f_cache = (seq_t *)realloc(f_cache, new_cap * sizeof(seq_t));
        f_used  = (unsigned char *)realloc(f_used,  new_cap * sizeof(unsigned char));

        if (a_used && old_a_cap < new_cap)
            memset(a_used + old_a_cap, 0, (new_cap - old_a_cap) * sizeof(unsigned char));
        if (f_used && old_f_cap < new_cap)
            memset(f_used + old_f_cap, 0, (new_cap - old_f_cap) * sizeof(unsigned char));

        a_cap = new_cap;
        f_cap = new_cap;
    }

    /* Base value */
    if (n == 1) {
        if (!a_used[1]) {
            a_cache[1] = (seq_t)1;
            a_used[1]  = 1;
        }
        return a_cache[1];
    }

    /* Return cached value if available */
    if (a_used[n])
        return a_cache[n];

    /* Compute the sum over proper divisors */
    seq_t sum = 0;

    /* Factorisation of n */
    uint64_t pr[64], ex[64];
    size_t pcnt;
    factor((uint64_t)n, pr, ex, &pcnt);

    /* Generate all divisors */
    uint64_t divs[512];
    size_t dcnt = 0;
    gen_divs_recursive(1, 0, pr, ex, pcnt, divs, &dcnt);

    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d = divs[i];
        if (d == n || d == 0)
            continue;

        seq_t ad = a(d);            /* recursive call – cached if needed */

        uint64_t q = n / d;
        seq_t fq;
        if (q == 1)
            fq = (seq_t)1;
        else if (f_used[q])
            fq = f_cache[q];
        else {
            fq = compute_f(q);
            f_cache[q] = fq;
            f_used[q]  = 1;
        }

        sum += ad * fq;
    }

    a_cache[n] = -sum;
    a_used[n]  = 1;
    return a_cache[n];
}
