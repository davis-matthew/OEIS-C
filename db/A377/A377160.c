/*
 * A377160  Numbers k such that A000217(k) is not cubefree.
 * https://oeis.org/A377160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 52, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   build_primes, ensure_capacity, floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth element of OEIS A377160 – the set of
   positive integers k for which the triangular number T(k)=k*(k+1)/2
   is NOT cube‑free.  The function memoises results so it works
   correctly for repeated or out‑of‑order calls. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_cube_root.h"
#include "../utils/build_primes.h"
#include "../utils/ensure_capacity.h"

static seq_t *terms = NULL;      /* memoised list of k */
static size_t terms_len = 0;     /* number of terms stored */
static size_t terms_cap = 0;     /* capacity of 'terms' array */
static idx_t next_k = 1;         /* next k to test */

static idx_t *prime_list = NULL;/* array of small primes */
static size_t prime_cnt = 0;     /* number of primes */

static void initialise_primes(void)
{
    if (prime_list) return;
    /* Sufficiently large limit ensures primes up to the cube root of
       triangular numbers for the expected range of k. */
    build_primes(200000, (idx_t **)&prime_list, &prime_cnt);
}

static bool not_cube_free(seq_t T, idx_t k)
{
    seq_t croot = floor_cube_root(T);
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        seq_t p = prime_list[pi];
        if (p > croot) break;

        seq_t exp = 0;
        /* exponent in k */
        idx_t tmp = k;
        while (tmp % (idx_t)p == 0) { ++exp; tmp /= (idx_t)p; }
        /* exponent in k+1 */
        tmp = k + 1;
        while (tmp % (idx_t)p == 0) { ++exp; tmp /= (idx_t)p; }

        if (p == 2) exp -= 1;       /* division by 2 in T(k) */
        if (exp >= 3) return true;  /* not cube‑free */
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* out of range; undefined in OEIS, return 0 */

    initialise_primes();

    /* Extend memoised terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        seq_t T = (seq_t)next_k * ((seq_t)next_k + 1) / 2;

        if (not_cube_free(T, next_k)) {
            /* store k */
            ensure_capacity(terms_len + 1, &terms_cap,
                            (void **)&terms, sizeof(seq_t));
            terms[terms_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return terms[n - 1];
}
