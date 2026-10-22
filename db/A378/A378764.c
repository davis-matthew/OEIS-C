/*
 * A378764  Sum of the semiprimes which are less than or equal to n minus the
 *          sum of the primes which are less than or equal to n.
 * https://oeis.org/A378764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (sum of semiprimes ≤ n) – (sum of primes ≤ n)
 * Range:     i8 n <= 38, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a lazy linear sieve to compute prefix sums of primes and
   semiprimes. Fits all required C11 / C++17 constraints. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* --------------------------------------------------------------------- */
/*  Static state for memoised evaluation                                 */
/* --------------------------------------------------------------------- */
static idx_t max_n = 0;          /* largest n for which tables are up to date */
static seq_t *prime_sum = NULL;  /* prefix sums of primes               */
static seq_t *sem_sum   = NULL;  /* prefix sums of semiprimes           */

/* --------------------------------------------------------------------- */
/*  Extend tables up to at least 'limit'.                                */
/* --------------------------------------------------------------------- */
static void extend_upto(idx_t limit)
{
    if (limit <= max_n)
        return;  /* already sufficient */

    /* --------------------------------------------------------------- */
    /* 1. Sieve of Eratosthenes with linear smallest‑prime‑factor.   */
    /* --------------------------------------------------------------- */
    bool *is_prime = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_prime)
        return;
    /* Prime status: mark positions 2 .. limit as true initially.      */
    if (limit >= 2)
        memset(is_prime + 2, true, (size_t)(limit - 1) * sizeof(bool));

    uint64_t *spf = (uint64_t *)calloc((size_t)(limit + 1), sizeof(uint64_t));
    if (!spf) { free(is_prime); return; }

    uint64_t *primes = (uint64_t *)calloc((size_t)(limit + 1), sizeof(uint64_t));
    if (!primes) { free(is_prime); free(spf); return; }
    size_t prime_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[(size_t)i]) {
            primes[prime_cnt++] = (uint64_t)i;
            spf[(size_t)i] = i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint64_t p  = primes[j];
            idx_t    ip = i * p;
            if (ip > limit)
                break;
            is_prime[(size_t)ip] = false;
            spf[(size_t)ip] = p;
            if (i % p == 0)
                break;
        }
    }

    /* --------------------------------------------------------------- */
    /* 2. Allocate and compute prefix sums.                           */
    /* --------------------------------------------------------------- */
    seq_t *prime_sum_new = alloc_seq_array((size_t)(limit + 1));
    seq_t *sem_sum_new   = alloc_seq_array((size_t)(limit + 1));
    if (!prime_sum_new || !sem_sum_new) {
        free(is_prime);
        free(spf);
        free(primes);
        free_seq_array(prime_sum_new);
        free_seq_array(sem_sum_new);
        return;
    }
    prime_sum_new[0] = 0;
    sem_sum_new[0]   = 0;

    for (idx_t i = 1; i <= limit; ++i) {
        prime_sum_new[i] = prime_sum_new[i-1];
        sem_sum_new[i]   = sem_sum_new[i-1];

        if (is_prime[(size_t)i]) {
            prime_sum_new[i] += (seq_t)i;
        } else { /* test for semiprime */
            if (i >= 4) {
                uint64_t p = spf[(size_t)i];
                uint64_t q = i / p;
                if (is_prime[(size_t)p] && is_prime[(size_t)q])
                    sem_sum_new[i] += (seq_t)i;
            }
        }
    }

    /* --------------------------------------------------------------- */
    /* 3. Replace old tables, freeing memory.                          */
    /* --------------------------------------------------------------- */
    free(is_prime);
    free(spf);
    free(primes);

    free_seq_array(prime_sum);
    free_seq_array(sem_sum);

    prime_sum = prime_sum_new;
    sem_sum   = sem_sum_new;
    max_n     = limit;
}

/* --------------------------------------------------------------------- */
/*  Main sequence function required by the evaluation harness.           */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;            /* undefined for n < 1; return 0 safely          */
    extend_upto(n);           /* ensure tables cover n at least once           */
    return sem_sum[(size_t)n] - prime_sum[(size_t)n];
}
