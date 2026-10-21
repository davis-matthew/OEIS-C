/*
 * A279617  Partial sums of A279481.
 * https://oeis.org/A279617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -1 + Σ_{k=1..n} (Σ_{i=2..k} A010051(i) * A010051(2k-i) *
 *            (π(2k-i)-π(i-1))) for n > 2 where A010051(n) = 1 if n is prime,
 *            0 otherwise. The initial values are: a(1)=0, a(2)=0
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static sieve data */
static idx_t sieve_limit = 0;
static uint8_t *is_prime = NULL;
static idx_t *pi_prefix = NULL;

/* Build sieve up to 'limit' (inclusive) */
static void build_sieve(idx_t limit)
{
    /* Free old */
    if (is_prime) free(is_prime);
    if (pi_prefix) free(pi_prefix);

    is_prime = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
    pi_prefix = (idx_t*)malloc((limit + 1) * sizeof(idx_t));
    if (!is_prime || !pi_prefix) {
        /* In case of allocation failure, abort */
        abort();
    }

    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= limit; j += p) {
                is_prime[j] = 0;
            }
        }
    }

    /* prefix prime counts */
    pi_prefix[0] = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        pi_prefix[i] = pi_prefix[i - 1] + (idx_t)is_prime[i];
    }

    sieve_limit = limit;
}

/* Ensure sieve data up to 2*n */
static void ensure_sieve(idx_t n)
{
    idx_t needed = 2 * n;
    if (needed <= sieve_limit) return;
    build_sieve(needed);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 2) return 0;

    ensure_sieve(n);

    seq_t total = 0;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t sumk = 0;
        for (idx_t i = 2; i <= k; ++i) {
            if (is_prime[i] && is_prime[2 * k - i]) {
                seq_t add = (seq_t)pi_prefix[2 * k - i] - (seq_t)pi_prefix[i - 1];
                sumk += add;
            }
        }
        total += sumk;
    }

    return total - 1;
}
