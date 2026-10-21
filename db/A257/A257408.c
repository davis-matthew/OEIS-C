/*
 * A257408  Values of n such that there is exactly 1 solution to x^2 - y^2 = n
 *          with x > y >= 0.
 * https://oeis.org/A257408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 1500, u32 n <= 1500, u64 n <= 1500,
 *            u128 n <= 1500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A257408: numbers n having exactly one representation
 *  x^2 - y^2 = n with x > y >= 0.
 * The sequence consists of
 *   1, and
 *   all odd primes,
 *   all numbers of the form 4*p where p is 1 or a prime.
 *
 * The function generates the n-th term by merging the sorted lists
 * of odd primes and 4×(1 or prime) and counting until the desired
 * index is reached.  It uses a single prime sieve executed once.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

#define PRIME_LIMIT 200000   /* sufficient for the required n */

/* global prime storage */
static uint64_t *global_primes = NULL;
static size_t   global_primes_cnt = 0;

/* ------ simple prime sieve ------------------------------------- */
static void generate_primes(uint64_t limit, uint64_t **out, size_t *cnt_out)
{
    size_t i, j;
    uint8_t *is_prime = (uint8_t *)malloc((size_t)(limit + 1));
    if (!is_prime) {
        *out = NULL;
        *cnt_out = 0;
        return;
    }

    for (i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    /* first pass: count primes to allocate array */
    *cnt_out = 0;
    for (i = 2; i <= limit; ++i)
        if (is_prime[i]) (*cnt_out)++;

    *out = (uint64_t *)malloc((*cnt_out) * sizeof(uint64_t));
    if (!*out) {
        free(is_prime);
        *cnt_out = 0;
        return;
    }

    /* second pass: store primes */
    size_t k = 0;
    for (i = 2; i <= limit; ++i)
        if (is_prime[i]) (*out)[k++] = i;

    free(is_prime);
}

/* initialize global primes once */
static void init_primes(void)
{
    if (!global_primes) {
        generate_primes((uint64_t)PRIME_LIMIT, &global_primes, &global_primes_cnt);
    }
}

/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* safety for offset 1 */
    if (n == 1) return (seq_t)1;            /* the first term */

    init_primes();                          /* ensure primes are ready */

    /* indices for generating merged sequence */
    size_t odd_prime_idx = 1;               /* point to primes[1] = 3 */
    size_t p4_idx        = 0;               /* point to primes[0] = 2 */
    bool   inserted_one = false;            /* whether 4*1 has been used */

    unsigned long long cnt = 1;             /* count includes the term 1 already */

    while (1) {
        /* first candidate: next odd prime (skip 2) */
        uint64_t cand1 = (odd_prime_idx < global_primes_cnt)
                             ? global_primes[odd_prime_idx] : ULLONG_MAX;
        /* second candidate: 4*1 or 4*prime[p4_idx] */
        uint64_t cand2;
        if (!inserted_one) {
            cand2 = 4ULL;
        } else if (p4_idx < global_primes_cnt) {
            cand2 = 4ULL * global_primes[p4_idx];
        } else {
            cand2 = ULLONG_MAX;
        }

        /* choose the smaller candidate */
        uint64_t next;
        bool chose_cand1;
        if (cand1 <= cand2) { next = cand1; chose_cand1 = true; }
        else { next = cand2; chose_cand1 = false; }

        /* if this is the n-th element, return it */
        if (cnt + 1 == n) return (seq_t)next;

        /* advance the pointer that was used */
        if (chose_cand1) {
            odd_prime_idx++;
        } else {
            if (!inserted_one) inserted_one = true;
            else p4_idx++;
        }

        cnt++;
    }
}
