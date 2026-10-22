/*
 * A333491  First index of partially unequal prime quartets.
 * https://oeis.org/A333491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): index of the first prime in the n‑th partially unequal prime quartet.
 * a(n) is the 1‑based starting index of four consecutive primes
 * (prime(k), prime(k+1), prime(k+2), prime(k+3)) such that
 * g(k) != g(k+1) != g(k+2), where g(i) = prime(i+1) - prime(i).
 * The function generates primes incrementally, tests each new quartet,
 * and memoises the indices so that calls in any order are fast. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"

/* static cache of generated primes */
static uint64_t *prime_arr = NULL;
static size_t   prime_cap = 0;
static size_t   prime_cnt = 0;

/* static cache of quartet starting indices */
static idx_t *quart_arr = NULL;
static size_t  quart_cap = 0;
static size_t  quart_cnt = 0;

static void ensure_prime_capacity(void)
{
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 8;
        uint64_t *tmp = (uint64_t *)realloc(prime_arr, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        prime_arr = tmp;
        prime_cap = new_cap;
    }
}

static void ensure_quart_capacity(void)
{
    if (quart_cnt == quart_cap) {
        size_t new_cap = quart_cap ? quart_cap * 2 : 8;
        idx_t *tmp = (idx_t *)realloc(quart_arr, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        quart_arr = tmp;
        quart_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Generate primes and detect quartets until we have n entries. */
    while (quart_cnt < n) {
        uint64_t candidate;
        if (prime_cnt == 0) {
            candidate = 2;
        } else {
            candidate = prime_arr[prime_cnt - 1] + 1;
        }

        /* Find the next prime */
        while (!isPrimeGeneric((seq_t)candidate)) {
            candidate++;
        }

        /* Store new prime */
        ensure_prime_capacity();
        prime_arr[prime_cnt++] = candidate;

        /* Once we have at least four primes, test the last quartet that ends at this prime */
        if (prime_cnt >= 4) {
            uint64_t gap1 = prime_arr[prime_cnt - 3] - prime_arr[prime_cnt - 4];
            uint64_t gap2 = prime_arr[prime_cnt - 2] - prime_arr[prime_cnt - 3];
            uint64_t gap3 = prime_arr[prime_cnt - 1] - prime_arr[prime_cnt - 2];

            if (gap1 != gap2 && gap2 != gap3) {
                idx_t start_idx = (idx_t)(prime_cnt - 3);  /* 1‑based start index */

                ensure_quart_capacity();
                quart_arr[quart_cnt++] = start_idx;
            }
        }
    }

    return (seq_t)quart_arr[n - 1];
}
