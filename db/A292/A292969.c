/*
 * A292969  Expansion of e.g.f. exp(x^4 * exp(-x)).
 * https://oeis.org/A292969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/4)} (-k)^(n-4*k)/(k! * (n-4*k)!)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* Compute integer power base^exp for any seq_t type. */
static seq_t int_pow(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* Sequence A292969. */
seq_t a(idx_t n)
{
    /* Allocate factorial table up to n. */
    size_t sz = (size_t)n + 1;
    seq_t *fact = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!fact) return (seq_t)0;            /* out‑of‑memory safeguard */

    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) fact[i] = fact[i-1] * (seq_t)i;

    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= n / 4; ++k)
    {
        idx_t exp   = n - 4 * k;
        seq_t base  = (seq_t)(- (int64_t)k);   /* signed k */
        seq_t power = int_pow(base, exp);

        seq_t term = fact[n];
        term = term / fact[k];
        term = term / fact[exp];
        term = term * power;

        sum = sum + term;
    }

    free(fact);
    return sum;
}
