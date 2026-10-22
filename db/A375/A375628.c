/*
 * A375628  Expansion of e.g.f. exp(2*x^3) / (1 - x * exp(x^3)).
 * https://oeis.org/A375628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/3⌋} ((n-3k+2)^k / k!)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Local exponentiation by squaring; works for any seq_t type. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Cache of factorials up to the largest n requested so far. */
static seq_t *fact_cache = NULL;
static size_t fact_len = 0;   /* number of factorials stored (size of array). */

/* Ensure that factorial values up to n are available in fact_cache. */
static void ensure_factorial(idx_t n)
{
    if (fact_len > (size_t)n)         /* enough values already available. */
        return;

    size_t new_len = (size_t)(n + 1);  /* need indices 0 .. n inclusive */

    seq_t *new_cache;
    if (!fact_cache)
        new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    else
        new_cache = (seq_t *)realloc(fact_cache, new_len * sizeof(seq_t));

    if (!new_cache)
        abort();                       /* Allocation failed – abort. */

    /* Initialize factorials from old length up to n. */
    if (!fact_cache) {
        new_cache[0] = 1; /* 0! = 1 */
        for (size_t i = 1; i <= n; ++i)
            new_cache[i] = new_cache[i - 1] * (seq_t)i;
    } else {
        for (size_t i = fact_len; i <= n; ++i)
            new_cache[i] = new_cache[i - 1] * (seq_t)i;
    }

    fact_cache = new_cache;
    fact_len  = new_len;
}

/* Sequence entry point. */
seq_t a(idx_t n)
{
    /* Make sure needed factorials are available. */
    ensure_factorial(n);
    seq_t fact_n = fact_cache[(size_t)n];

    seq_t sum = 0;
    idx_t kmax = n / 3;                    /* floor(n/3) */

    for (idx_t k = 0; k <= kmax; ++k) {
        seq_t base = (seq_t)(n - 3 * k + 2);   /* n - 3k + 2 ≥ 2 */
        seq_t pow_val = pow_seq(base, k);      /* (n-3k+2)^k */
        seq_t fact_k = fact_cache[(size_t)k];  /* k! */
        seq_t fact_div = fact_n / fact_k;      /* n! / k! */
        seq_t term = fact_div * pow_val;       /* (n! / k!) * (n-3k+2)^k */
        sum += term;
    }

    return sum;
}
