/*
 * A332513  Numbers k such that phi(k) == 4 (mod 12), where phi is the Euler
 *          totient function (A000010).
 * https://oeis.org/A332513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9306 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A332513: numbers k such that φ(k) ≡ 4 (mod 12) */
/* The function a(idx_t n) returns the n‑th term (1‑based) */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

/* --------- linear sieve for φ and prime list --------- */
static void compute_phi_and_primes(idx_t limit,
                                   seq_t *phi,
                                   idx_t **primes,
                                   size_t *primes_cnt)
{
    /* is_prime boolean array */
    bool *is_prime = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_prime) exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = (i >= 2);

    /* prime list */
    *primes = (idx_t *)malloc(limit * sizeof(idx_t));
    if (!(*primes)) { free(is_prime); exit(EXIT_FAILURE); }
    *primes_cnt = 0;

    phi[0] = 0;
    phi[1] = 1;

    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            (*primes)[(*primes_cnt)++] = i;
            phi[i] = ((seq_t)i) - 1;
        }
        for (size_t pi = 0; pi < *primes_cnt; ++pi) {
            idx_t p = (*primes)[pi];
            idx_t val = i * p;
            if (val > limit) break;
            is_prime[val] = false;
            if (i % p == 0) {
                phi[val] = phi[i] * ((seq_t)p);
                break;
            } else {
                phi[val] = phi[i] * ((seq_t)(p - 1));
            }
        }
    }
    free(is_prime);
}

extern seq_t phi(idx_t m);           /* provided utility (unused here but kept for consistency) */

/* --------- static state --------- */
static idx_t   current_limit = 0;    /* highest integer for which we have built the list */
static seq_t *seq_cache    = NULL;   /* cache of found terms */
static size_t seq_size     = 0;      /* number of terms stored */

/* --------- rebuild the sequence up to new_limit --------- */
static void extend_to(idx_t new_limit)
{
    /* allocate φ array */
    seq_t *phi = mem_new(seq_t, (new_limit + 1));
    if (!phi) exit(EXIT_FAILURE);

    /* compute φ and prime list */
    idx_t *primes = NULL;
    size_t primes_cnt = 0;
    compute_phi_and_primes(new_limit, phi, &primes, &primes_cnt);

    /* build new cache */
    size_t cap = new_limit / 10 + 10;   /* initial guess */
    seq_t *new_cache = mem_new(seq_t, cap);
    if (!new_cache) { mem_free(phi); free(primes); exit(EXIT_FAILURE); }
    size_t new_size = 0;

    for (idx_t k = 1; k <= new_limit; ++k) {
        if ((phi[k] % 12) == 4) {
            if (new_size == cap) {          /* grow if necessary */
                cap *= 2;
                seq_t *tmp = mem_resize(seq_t, new_cache, cap);
                if (!tmp) { mem_free(phi); free(primes); mem_free(new_cache); exit(EXIT_FAILURE); }
                new_cache = tmp;
            }
            new_cache[new_size++] = k;
        }
    }

    /* replace old cache */
    mem_free(seq_cache);
    seq_cache    = new_cache;
    seq_size     = new_size;
    current_limit = new_limit;

    mem_free(phi);
    free(primes);
}

/* --------- public sequence function --------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for 0, guard only */

    /* ensure we have at least n terms */
    if (seq_size < (size_t)n) {
        idx_t limit = (current_limit > 0) ? current_limit : 200;
        while (seq_size < (size_t)n) {
            if (limit < 200) limit = 200;
            else             limit *= 2;
            extend_to(limit);
        }
    }

    return seq_cache[n - 1];
}
