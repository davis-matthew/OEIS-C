/*
 * A392348  Primes p such that the Chebyshev distance from 1 to p in the Ulam
 *          spiral is not a prime number.
 * https://oeis.org/A392348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime p such that floor((sqrt(p - 1) + 1) / 2) is not
 *            prime
 * Range:     u8 n <= 7, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   alloc_seq_array, floor_sqrt, grow_seq_array, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"    /* floor_sqrt(seq_t x) */
#include "../utils/is_prime_big.h"  /* bool is_prime_big(seq_t n) */
#include "../utils/alloc_seq_array.h"   /* seq_t *alloc_seq_array(size_t n) */
#include "../utils/grow_seq_array.h"    /* void grow_seq_array(seq_t **arr, size_t *size, size_t new_len) */

seq_t a(idx_t n)
{
    /* Static state for memoisation and progress */
    static seq_t *selected = NULL;   /* array of accepted primes */
    static size_t sel_cnt  = 0;      /* number of accepted primes found so far */
    static size_t sel_cap  = 0;      /* capacity of selected array */
    static seq_t  next_candidate = 2;   /* next integer to test for primality */

    /* Return already found term if available */
    if (n <= sel_cnt) {
        return selected[(size_t)(n - 1)];
    }

    /* Generate and test primes until we have n terms */
    while (sel_cnt < n) {
        seq_t cand = next_candidate;

        /* Test whether cand is prime */
        if (is_prime_big(cand)) {
            /* Compute k = (floor_sqrt(cand - 1) + 1) / 2 */
            seq_t r = floor_sqrt(cand - (seq_t)1);
            seq_t k = (r + (seq_t)1) / (seq_t)2;

            /* Accept cand if k is not prime (i.e., composite or 1) */
            if (!is_prime_big(k)) {
                /* Grow the selected array if necessary */
                grow_seq_array(&selected, &sel_cap, sel_cnt + (size_t)1);
                selected[sel_cnt++] = cand;
            }
        }

        /* Advance to the next candidate integer */
        ++next_candidate;
    }

    /* Return the requested term */
    return selected[(size_t)(n - 1)];
}
