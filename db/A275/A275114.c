/*
 * A275114  Primes p for which the sum of the numbers in the Collatz iteration
 *          (A033493) of p is a prime.
 * https://oeis.org/A275114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 9, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A275114 : primes p such that the sum of the
 * Collatz trajectory of p is a prime number.  The function does
 * not use a closed formula but searches primes and tests the
 * Collatz sum on demand, memoising results.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"   /* bool is_prime_small(uint64_t n); */
#include "../utils/is_prime_big.h"     /* bool is_prime_big(seq_t n); */

static seq_t *seq_list = NULL;   /* stored primes whose Collatz sum is prime */
static size_t seq_len  = 0;      /* number of stored terms */
static size_t seq_cap  = 0;      /* array capacity */

static uint64_t cand = 1;        /* next integer to test for primality */

/* Ensure seq_list can hold at least 'min_cap' elements */
static void ensure_seq_cap(size_t min_cap)
{
    if (seq_cap < min_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 4;
        while (new_cap < min_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(seq_list, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        seq_list = tmp;
        seq_cap  = new_cap;
    }
}

/* Extend seq_list until it contains at least 'target' terms */
static void extend_to(idx_t target)
{
    while (seq_len < (size_t)target) {
        /* Find next prime candidate */
        do { ++cand; } while (!is_prime_small(cand));

        /* Compute the sum of the Collatz trajectory of this prime */
        seq_t sum = (seq_t)cand;
        seq_t x   = (seq_t)cand;
        while (x != (seq_t)1) {
            if ((x % (seq_t)2) == (seq_t)0) {
                x = x / (seq_t)2;
            } else {
                x = (seq_t)3 * x + (seq_t)1;
            }
            sum += x;
        }

        /* If the sum is prime, record the prime itself */
        if (is_prime_big(sum)) {
            ensure_seq_cap(seq_len + 1);
            seq_list[seq_len++] = (seq_t)cand;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* outside the OEIS offset */
    extend_to(n);
    return seq_list[n - 1];
}
