/*
 * A254672  Primes prime(n) such that prime(n) + 6*n is also prime.
 * https://oeis.org/A254672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) such that prime(n) + 6*n is prime
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

static seq_t *valid_arr   = NULL;   /* stored valid primes */
static size_t valid_count = 0;
static size_t valid_cap   = 0;

static uint64_t last_prime_checked = 1;  /* last prime found, start at 1 */
static idx_t    prime_index = 0;         /* 1‑based index of last_prime_checked */

/* Append a new valid prime to the storage array. */
static void append_valid(seq_t v)
{
    if (valid_count == valid_cap) {
        size_t new_cap = valid_cap ? valid_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(valid_arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();     /* unlikely, but safe cleanup */
        valid_arr = tmp;
        valid_cap = new_cap;
    }
    valid_arr[valid_count++] = v;
}

/* Ensure that at least 'target' valid primes have been found. */
static void ensure_valid_count(idx_t target)
{
    while (valid_count < (size_t)target) {
        /* Find the next prime after last_prime_checked. */
        uint64_t cand = last_prime_checked + 1;
        if (cand <= 2) {
            cand = 2;
        }
        if (cand == 2) {
            last_prime_checked = 2;
        } else {
            if ((cand & 1ULL) == 0) cand++;  /* make odd */
            while (!is_prime((idx_t)cand)) {
                cand += 2;                   /* skip even numbers */
            }
            last_prime_checked = cand;
        }
        prime_index++;                         /* prime number's index */

        /* Test if prime + 6*index is prime. */
        uint64_t check = (uint64_t)last_prime_checked + 6ULL * (uint64_t)prime_index;
        if (is_prime((idx_t)check)) {
            append_valid((seq_t)last_prime_checked);
        }
    }
}

/* Return the n‑th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset not used; safety return 0 */
    ensure_valid_count(n);
    return valid_arr[(size_t)n - 1];
}
