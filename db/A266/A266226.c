/*
 * A266226  a(n) = floor(Sum_{d|n} 1 / tau(d)).
 * https://oeis.org/A266226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(Sum_{d|n} 1 / tau(d))
 * Range:     u8 n <= 35, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* idx_t      */
#include <stdint.h>   /* uint64_t   */
#include <stdlib.h>   /* malloc,realloc */

/* GCD for the integer type seq_t */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a    = b;
        b    = t;
    }
    return a;
}

/* Cache the divisor‑count values tau[1..N] */
static seq_t *tau_cache = NULL;
static idx_t tau_cache_size = 0;

/* Recompute tau[1..limit] using a simple sieve */
static void compute_tau_up_to(idx_t limit)
{
    if (limit < 1) return;
    idx_t size = limit + 1;

    tau_cache = (seq_t *)realloc(tau_cache, size * sizeof(seq_t));
    for (idx_t i = 0; i < size; ++i) {
        tau_cache[i] = (seq_t)0;
    }

    for (idx_t i = 1; i <= limit; ++i) {
        for (idx_t j = i; j <= limit; j += i) {
            tau_cache[j] += (seq_t)1;
        }
    }
    tau_cache_size = limit;
}

/* Helper: add the fraction 1/tau_d to the running sum num/den. */
static void add_fraction(seq_t *num, seq_t *den, seq_t tau_d)
{
    seq_t new_num = (*num) * tau_d + (*den);
    seq_t new_den = (*den) * tau_d;
    seq_t g = gcd_seq(new_num, new_den);
    new_num /= g;
    new_den /= g;
    *num = new_num;
    *den = new_den;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)   /* defined only for n ≥ 1 */
        return (seq_t)0;

    if (n > tau_cache_size)
        compute_tau_up_to(n);

    seq_t sum_num = (seq_t)0;
    seq_t sum_den = (seq_t)1;

    /* Enumerate divisors of n */
    for (idx_t i = 1; (idx_t)i * i <= n; ++i) {
        if (n % i == 0) {
            seq_t tau_i   = tau_cache[i];
            add_fraction(&sum_num, &sum_den, tau_i);

            idx_t j = n / i;
            if (j != i) {
                seq_t tau_j   = tau_cache[j];
                add_fraction(&sum_num, &sum_den, tau_j);
            }
        }
    }

    seq_t result = sum_num / sum_den;   /* floor of the rational sum */
    return result;
}
