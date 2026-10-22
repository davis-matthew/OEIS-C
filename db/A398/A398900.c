/*
 * A398900  Prime numbers that can be written as half the sum of three squares
 *          in exactly one unique way.
 * https://oeis.org/A398900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, changed
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   floor_sqrt, sieve_small_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computation of sequence A398900: primes p such that 2p can be written as
   a sum of three squares in exactly one unordered triple (x≥y≥z≥0).
   The sequence is finite (only 8 terms).  The function searches all
   primes up to a modest limit (200) and caches the results for fast
   repeated calls.  A simple sieve of Eratosthenes is used to generate
   the primes.
*/
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"
#include "../utils/sieve_small_primes.h"

static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

static void grow_terms(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = (terms_cap > 0) ? (terms_cap << 1) : 4;
    while (needed > new_cap) new_cap <<= 1;
    terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (terms == NULL) {
        /* Initial calculation of all terms. */
        uint64_t *prime_list = NULL;
        size_t prime_cnt = 0;
        sieve_small_primes(200, &prime_list, &prime_cnt);

        for (size_t idx = 0; idx < prime_cnt; ++idx) {
            seq_t p = (seq_t)prime_list[idx];
            seq_t target = p * (seq_t)2;      /* 2p */
            idx_t max_xyz = (idx_t)floor_sqrt(target);
            size_t reps = 0;

            for (idx_t x = 0; x <= max_xyz && reps <= 1; ++x) {
                seq_t x_sq = (seq_t)x * x;
                if (x_sq > target) break;
                for (idx_t y = 0; y <= x && reps <= 1; ++y) {
                    seq_t y_sq = (seq_t)y * y;
                    seq_t sum_xy = x_sq + y_sq;
                    if (sum_xy > target) break;
                    seq_t z_sq = target - sum_xy;
                    seq_t z_root = floor_sqrt(z_sq);
                    if (z_root > (seq_t)y) continue;          /* enforce z ≤ y */
                    if (z_root * z_root == z_sq)
                        ++reps;
                }
            }
            if (reps == 1) {
                grow_terms(terms_len + 1);
                terms[terms_len++] = p;
            }
        }
        free(prime_list);
    }

    if ((idx_t)n <= terms_len)
        return terms[(idx_t)n - 1];
    return (seq_t)0;
}
