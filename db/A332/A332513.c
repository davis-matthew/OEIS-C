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
 * Helpers:   compute_phi_and_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A332513: numbers k such that φ(k) ≡ 4 (mod 12) */
/* The function a(idx_t n) returns the n‑th term (1‑based) */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/compute_phi_and_primes.h"

extern seq_t phi(idx_t m);           /* provided utility (unused here but kept for consistency) */

/* --------- static state --------- */
static idx_t   current_limit = 0;    /* highest integer for which we have built the list */
static seq_t *seq_cache    = NULL;   /* cache of found terms */
static size_t seq_size     = 0;      /* number of terms stored */

/* --------- rebuild the sequence up to new_limit --------- */
static void extend_to(idx_t new_limit)
{
    /* allocate φ array */
    seq_t *phi = (seq_t *)malloc((new_limit + 1) * sizeof(seq_t));
    if (!phi) exit(EXIT_FAILURE);

    /* compute φ and prime list */
    idx_t *primes = NULL;
    size_t primes_cnt = 0;
    compute_phi_and_primes(new_limit, phi, &primes, &primes_cnt);

    /* build new cache */
    size_t cap = new_limit / 10 + 10;   /* initial guess */
    seq_t *new_cache = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!new_cache) { free(phi); free(primes); exit(EXIT_FAILURE); }
    size_t new_size = 0;

    for (idx_t k = 1; k <= new_limit; ++k) {
        if ((phi[k] % 12) == 4) {
            if (new_size == cap) {          /* grow if necessary */
                cap *= 2;
                seq_t *tmp = (seq_t *)realloc(new_cache, cap * sizeof(seq_t));
                if (!tmp) { free(phi); free(primes); free(new_cache); exit(EXIT_FAILURE); }
                new_cache = tmp;
            }
            new_cache[new_size++] = k;
        }
    }

    /* replace old cache */
    free(seq_cache);
    seq_cache    = new_cache;
    seq_size     = new_size;
    current_limit = new_limit;

    free(phi);
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
