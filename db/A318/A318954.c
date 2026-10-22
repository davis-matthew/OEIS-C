/*
 * A318954  Minimum shifted Heinz number of a strict factorization of n into
 *          factors > 1.
 * https://oeis.org/A318954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimum product of prime(y_i‑1) over all strict
 *            factorizations of n into factors y_i>1 (factors strictly
 *            increasing)
 * Range:     u8 n <= 55, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Primes are stored 0‑based: primes[0] = 2 (prime(1)), primes[1] = 3, ... */
static uint64_t *primes = NULL;
static size_t   prime_cnt = 0;
static bool     primes_ready = false;

/* Build primes up to a fixed limit that covers all needed indices
   (n ≤ 10 000 → we need prime(9999) < 104 729). */
static void init_primes(void)
{
    if (primes_ready) return;

    const size_t limit = 200000;           /* safe upper bound for prime(9999) */
    bool *is_prime = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_prime) return;

    /* True for numbers ≥2, false for 0 and 1 */
    for (size_t i = 2; i <= limit; ++i) is_prime[i] = true;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i) {
                is_prime[j] = false;
            }
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!primes) {
        free(is_prime);
        return;
    }

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[idx++] = (uint64_t)i;

    free(is_prime);
    prime_cnt = cnt;
    primes_ready = true;
}

/* Recursive search: rem = remaining number to factor,
   min_f = smallest allowed factor (to enforce strict increase),
   prod = current product of prime(y_i-1),
   best = pointer to current best product. */
static void dfs(uint64_t rem, uint64_t min_f, seq_t prod, seq_t *best)
{
    if (rem == 1) {                       /* a complete factorization */
        if (*best == 0 || prod < *best) *best = prod;
        return;
    }
    if (*best && prod >= *best) return;   /* pruning */

    /* Iterate over all divisors of rem that are ≥ min_f */
    for (uint64_t d = min_f; d <= rem; ++d) {
        if (rem % d != 0) continue;       /* not a divisor */
        /* prime(d-1) -> primes[d-2] (since primes[0]=prime(1)=2) */
        seq_t next_prod = prod * (seq_t)primes[d - 2];
        dfs(rem / d, d + 1, next_prod, best);
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;   /* offset 1: a(1)=1 */

    init_primes();

    seq_t best = 0;
    dfs((uint64_t)n, 2, (seq_t)1, &best);
    return best;
}
