/*
 * A388658  Primes not of the form (prime(k-1)*prime(k+1)) mod prime(k).
 * https://oeis.org/A388658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 5158, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate primes that never appear as
   (prime(k-1) * prime(k+1)) mod prime(k). */

/* Types seq_t and idx_t are provided by the build; do not redefine them. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Static storage for memoised result */
static seq_t *result = NULL;
static idx_t  result_len = 0;

/* One‑time precomputation of the sequence up to a safe bound. */
static void precompute(void)
{
    if (result != NULL) return;          /* already computed */

    /* Upper bound – sufficient for the first 10 000 terms. */
    const idx_t LIMIT = 10000000;         /* 10⁷ */

    /* ---------- sieve of Eratosthenes ---------- */
    char *is_prime = (char*)calloc(LIMIT + 1, sizeof(char));
    if (!is_prime) exit(1);

    /* Initially mark every number ≥2 as prime. */
    for (idx_t i = 2; i <= LIMIT; ++i) is_prime[i] = 1;

    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = (idx_t)(i * i); j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes for allocation. */
    idx_t prime_cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++prime_cnt;

    /* Store the primes in an array. */
    uint64_t *primes = (uint64_t*)malloc(prime_cnt * sizeof(uint64_t));
    if (!primes) { free(is_prime); exit(1); }
    idx_t pidx = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) primes[pidx++] = (uint64_t)i;

    /* ---------- mark residues that are primes ---------- */
    char *excluded = (char*)calloc(LIMIT + 1, sizeof(char));
    if (!excluded) { free(primes); free(is_prime); exit(1); }

    for (idx_t k = 1; k + 1 < prime_cnt; ++k) {
        uint64_t a = primes[k - 1];
        uint64_t b = primes[k + 1];
        uint64_t p = primes[k];
        uint64_t prod = a * b;          /* fits in 64 bits (≤1e14) */
        uint64_t r = prod % p;
        if (r >= 2 && r <= LIMIT && is_prime[r])
            excluded[r] = 1;
    }

    /* ---------- build the result array ---------- */
    result = (seq_t*)malloc(prime_cnt * sizeof(seq_t));
    if (!result) { free(excluded); free(primes); free(is_prime); exit(1); }

    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i] && !excluded[i])
            result[result_len++] = (seq_t)i;
    }

    /* ---------- cleanup temporary arrays ---------- */
    free(is_prime);
    free(excluded);
    free(primes);
}

/* --------------------------------------------------------------- */
/* Public function: returns the n‑th term of the sequence (1‑based). */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    precompute();

    if (n < 1 || (idx_t)n > result_len)
        return 0;        /* out of bounds – not expected in tests */

    return result[n - 1];
}
