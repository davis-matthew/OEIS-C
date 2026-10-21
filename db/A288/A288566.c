/*
 * A288566  Partial sums of A087207.
 * https://oeis.org/A288566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p≤n} 2^(π(p)-1) * ⌊n/p⌋ (partial sums of A087207)
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 130, u64 n <= 310,
 *            u128 n <= 718, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2.h"

/* ------------------------------------------------------------------ */
/* Prime list storage (grows only as needed).                         */
/* ------------------------------------------------------------------ */
static idx_t *prime_list = NULL;   /* array of primes */
static size_t prime_cnt  = 0;       /* number of primes stored   */
static size_t prime_cap  = 0;       /* allocated capacity        */

/* ------------------------------------------------------------------ */
/* Ensure that all primes up to 'limit' are stored in prime_list.     */
/* ------------------------------------------------------------------ */
static void ensure_primes(idx_t limit)
{
    /* Nothing to do if we already have enough primes. */
    if (prime_cnt > 0 && prime_list[prime_cnt - 1] >= limit)
        return;

    idx_t start = (prime_cnt > 0) ? prime_list[prime_cnt - 1] + 1 : 2;

    for (idx_t cand = start; cand <= limit; ++cand) {
        bool is_prime = true;

        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = prime_list[i];
            /* stop if p^2 > cand, using division to avoid overflow */
            if (p > cand / p)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* grow the array if necessary */
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                idx_t *tmp = (idx_t *)realloc(prime_list,
                                              prime_cap * sizeof(idx_t));
                if (!tmp)
                    exit(1); /* exit on allocation failure */
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = cand;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the test harness.                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* sequence is defined for n≥1, but guard anyway */

    /* Make sure we have all primes up to n. */
    ensure_primes(n);

    seq_t sum = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p > n)
            break;
        seq_t term = pow2((idx_t)i);        /* 2^(π(p)-1) */
        term *= (seq_t)(n / p);             /* * floor(n/p) */
        sum += term;
    }
    return sum;
}
