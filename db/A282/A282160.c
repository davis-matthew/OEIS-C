/*
 * A282160  Least k > 1 such that k*n is not a totient number.
 * https://oeis.org/A282160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 6719, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the least k > 1 such that k*n is NOT a totient number. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* static state for the totient sieve */
static idx_t cur_limit = 0;
static uint64_t *phi = NULL;
static bool *is_totient = NULL;

/* Build the totient values up to at least NEW_LIMIT. */
static void rebuild_sieve(idx_t new_limit)
{
    /* free previous data */
    free(phi);
    free(is_totient);

    /* allocate new arrays */
    phi = (uint64_t *)malloc(sizeof(uint64_t) * (new_limit + 1));
    is_totient = (bool *)malloc(sizeof(bool) * (new_limit + 1));
    if (!phi || !is_totient)
        return;                       /* allocation failed – leave empty */

    /* initialise phi(i) = i */
    for (idx_t i = 0; i <= new_limit; ++i)
        phi[i] = (uint64_t)i;

    /* Euler's sieve for φ */
    for (idx_t i = 2; i <= new_limit; ++i)
        if (phi[i] == (uint64_t)i)                 /* i is prime */
            for (idx_t j = i; j <= new_limit; j += i)
                phi[j] -= phi[j] / i;

    /* mark totient values */
    memset(is_totient, 0, sizeof(bool) * (new_limit + 1));
    for (idx_t i = 0; i <= new_limit; ++i) {
        uint64_t v = phi[i];
        if (v <= (uint64_t)new_limit)
            is_totient[(size_t)v] = true;
    }

    cur_limit = new_limit;
}

/* Return a(n) for any n ≥ 1. */
seq_t a(idx_t n)
{
    /* initialise sieve lazily */
    if (cur_limit == 0)
        rebuild_sieve(1);                 /* minimal safe limit */

    /* find smallest k > 1 with n*k not in the totient set */
    for (idx_t k = 2;; ++k) {
        seq_t prod = (seq_t)n * (seq_t)k;    /* product */

        /* obtain an idx_t index for the product; assume it fits */
        idx_t prod_idx = (idx_t)prod;

        /* ensure the sieve covers up to prod (and a safety margin) */
        if (prod_idx > cur_limit || (prod_idx * (idx_t)2 > cur_limit)) {
            idx_t new_limit = cur_limit * 2;
            while (prod_idx * 2 > new_limit)
                new_limit *= 2;
            rebuild_sieve(new_limit);
        }

        /* test totientness */
        if (prod_idx <= cur_limit && !is_totient[(size_t)prod_idx])
            return (seq_t)k;
    }
}
