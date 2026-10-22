/*
 * A383679  The lesser of two consecutive primes whose gap equals the
 *          difference between their digital sums.
 * https://oeis.org/A383679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

/* A383679: lesser prime of consecutive primes such that the prime gap
   equals the difference of their digital sums.
   Computed by generating primes incrementally and testing the condition. */

/* Dynamic array of primes (uint64_t). */
static uint64_t *primes      = NULL;
static size_t   primes_cap   = 0;
static size_t   primes_cnt   = 0;

/* Dynamic array of sequence terms (seq_t). */
static seq_t *terms         = NULL;
static size_t  terms_cap    = 0;
static size_t  terms_cnt    = 0;

/* Current index of the left prime in the pair being examined. */
static size_t pair_idx = 0;

/* Ensure capacity for the primes array. */
static void ensure_primes_cap(size_t need)
{
    while (primes_cap < need) {
        size_t new_cap = primes_cap ? primes_cap << 1 : 256;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp) exit(1);
        primes = tmp;
        primes_cap = new_cap;
    }
}

/* Append the next prime to the primes array. */
static void add_prime(void)
{
    if (primes_cnt == 0) {
        ensure_primes_cap(1);
        primes[0] = 2;
        primes_cnt = 1;
        return;
    }

    uint64_t cand = primes[primes_cnt - 1] + 1;
    if ((cand & 1) == 0) cand++;   /* skip even numbers */

    for (;;) {
        bool is_prime = true;
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint64_t p = primes[j];
            unsigned __int128 sq = (unsigned __int128)p * p;
            if (sq > cand) break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            ensure_primes_cap(primes_cnt + 1);
            primes[primes_cnt++] = cand;
            return;
        }
        cand += 2;
    }
}

/* Ensure capacity for the terms array. */
static void ensure_terms_cap(size_t need)
{
    while (terms_cap < need) {
        size_t new_cap = terms_cap ? terms_cap << 1 : 256;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        terms = tmp;
        terms_cap = new_cap;
    }
}

/* Generate the sequence up to the desired 1‑based index. */
static void compute_up_to(idx_t target)
{
    if (target == 0) return;

    while (terms_cnt < (size_t)target) {
        /* Ensure we have a pair of consecutive primes ready. */
        while (primes_cnt <= pair_idx + 1) add_prime();

        uint64_t p1 = primes[pair_idx];
        uint64_t p2 = primes[pair_idx + 1];
        seq_t   ds1 = digit_sum((seq_t)p1);
        seq_t   ds2 = digit_sum((seq_t)p2);
        seq_t   gap = (seq_t)(p2 - p1);

        int diff = (int)ds2 - (int)ds1;
        if (diff > 0 && (seq_t)diff == gap) {
            ensure_terms_cap(terms_cnt + 1);
            terms[terms_cnt++] = (seq_t)p1;
        }
        pair_idx++;
    }
}

/* External linkage: returns the n‑th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined behaviour for n==0, though OEIS offset starts at 1 */
    compute_up_to(n);
    return terms[(size_t)(n - 1)];
}
