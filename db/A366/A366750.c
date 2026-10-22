/*
 * A366750  Number of strict integer partitions of n into odd parts with a
 *          common divisor > 1.
 * https://oeis.org/A366750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of strict partitions of n into odd parts with a
 *            common odd divisor > 1. Computed as a(n) = f(n) - g1(n), where
 *            f(n) is the number of partitions into distinct odd parts, and
 *            g1(n) = Σ_{d|n, d odd} μ(d) * f(n/d), μ is the Möbius function
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Forward declaration of types */
extern seq_t a(idx_t n);

static seq_t *f_cache = NULL;      /* array of f(n) for 0..max */
static int8_t *mu_cache = NULL;    /* Möbius values for 0..max */
static idx_t max_n_computed = 0;   /* current maximum n for which tables are built */

/* Build f and mu tables up to limit (inclusive) */
static void build_tables(idx_t limit)
{
    /* allocate new arrays */
    seq_t *new_f = (seq_t*)malloc((size_t)(limit + 1) * sizeof(seq_t));
    int8_t *new_mu = (int8_t*)malloc((size_t)(limit + 1) * sizeof(int8_t));
    if (!new_f || !new_mu) return; /* allocation failed; caller will handle */

    /* compute f: number of partitions into distinct odd parts */
    for (idx_t i = 0; i <= limit; ++i) new_f[i] = 0;
    new_f[0] = 1;
    for (idx_t k = 1; k <= limit; k += 2) {
        for (idx_t s = limit; s >= k; --s) {
            new_f[s] += new_f[s - k];
        }
    }

    /* compute Möbius function up to limit */
    /* initialize mu to 1 and is_prime array */
    for (idx_t i = 0; i <= limit; ++i) {
        new_mu[i] = 1;
    }
    new_mu[0] = 0; /* not used */
    new_mu[1] = 1;
    /* Sieve for primes and Möbius */
    uint8_t *is_prime = (uint8_t*)malloc((size_t)(limit + 1) * sizeof(uint8_t));
    if (!is_prime) {
        free(new_f);
        free(new_mu);
        return;
    }
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            /* i is prime */
            for (idx_t j = i; j <= limit; j += i) {
                is_prime[j] = 0;
                new_mu[j] = (int8_t)(-new_mu[j]); /* multiply by -1 */
                if ((j / i) % i == 0) {
                    new_mu[j] = 0; /* square factor */
                }
            }
        }
    }
    free(is_prime);

    /* free old tables if any */
    if (f_cache) free(f_cache);
    if (mu_cache) free(mu_cache);

    /* store new tables */
    f_cache = new_f;
    mu_cache = new_mu;
    max_n_computed = limit;
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* definition for a(0) */

    /* Ensure tables large enough */
    if (max_n_computed < n) {
        build_tables(n);
    }

    seq_t total = f_cache[n];

    seq_t pos_sum = 0;
    seq_t neg_sum = 0;

    /* Enumerate odd divisors of n */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* d is a divisor */
            if ((d & 1U) && d <= n) {
                int8_t mu = mu_cache[d];
                if (mu == 1) {
                    pos_sum += f_cache[n / d];
                } else if (mu == -1) {
                    neg_sum += f_cache[n / d];
                }
            }
            idx_t e = n / d;
            if (e != d && (e & 1U) && e <= n) {
                int8_t mu = mu_cache[e];
                if (mu == 1) {
                    pos_sum += f_cache[n / e];
                } else if (mu == -1) {
                    neg_sum += f_cache[n / e];
                }
            }
        }
    }

    seq_t g1 = pos_sum - neg_sum; /* partitions with gcd exactly 1 */
    return total - g1;            /* partitions with gcd > 1 */
}
