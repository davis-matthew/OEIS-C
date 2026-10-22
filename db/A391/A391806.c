/*
 * A391806  a(n) = least k such that the sum of the first k odd primes is
 *          divisible by n.
 * https://oeis.org/A391806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that the sum of the first k odd primes is
 *            divisible by n
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"   /* provides: uint64_t next_prime(uint64_t start); */

static uint64_t *odd_primes = NULL;    /* list of odd primes (3,5,7,11,...) */
static size_t odd_count   = 0;         /* number of odd primes currently stored */
static size_t odd_cap     = 0;         /* capacity of odd_primes array */

static seq_t *prefix_sums = NULL;     /* prefix_sums[k] = sum of first k odd primes, prefix_sums[0]==0 */
static size_t prefix_cap   = 0;        /* capacity of prefix_sums array (always odd_cap+1) */

/* Ensure the prime/partial-sum tables contain at least k odd primes (k>=1). */
static void ensure_prefix_sum(size_t k_needed)
{
    if (k_needed == 0) return;

    /* Initialize arrays on first use */
    if (odd_cap == 0) {
        odd_cap = 8;
        odd_primes = (uint64_t *)malloc(odd_cap * sizeof(uint64_t));
        prefix_cap = odd_cap + 1;
        prefix_sums = (seq_t *)malloc(prefix_cap * sizeof(seq_t));
        prefix_sums[0] = 0;   /* sum of zero primes */
    }

    /* Grow prime array if necessary */
    if (odd_count < k_needed) {
        size_t old_odd_cap = odd_cap;
        while (odd_count < k_needed) {
            uint64_t new_prime;
            if (odd_count == 0)
                new_prime = 3;
            else
                new_prime = next_prime(odd_primes[odd_count - 1]);

            /* grow capacity */
            if (odd_count == odd_cap) {
                odd_cap = odd_cap * 2;
                odd_primes = (uint64_t *)realloc(odd_primes,
                    odd_cap * sizeof(uint64_t));
                prefix_cap = odd_cap + 1;
                prefix_sums = (seq_t *)realloc(prefix_sums,
                    prefix_cap * sizeof(seq_t));
            }

            odd_primes[odd_count] = new_prime;
            seq_t prev_sum = prefix_sums[odd_count];
            seq_t new_sum  = prev_sum + (seq_t)new_prime;
            prefix_sums[odd_count + 1] = new_sum;
            odd_count++;
        }
    }
}

/* Cache for previously computed a(n) values. */
static seq_t *a_cache = NULL;         /* cache[n] holds a(n) for n>=1 */
static size_t  a_cache_size = 0;      /* allocated size of a_cache */

/* Return least k (>=1) such that sum of first k odd primes ≡ 0 (mod n). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for 0, but safe return */

    /* grow cache if necessary */
    if (n + 1 > a_cache_size) {
        size_t new_size = (n + 1) * 2;
        a_cache = (seq_t *)realloc(a_cache,
            new_size * sizeof(seq_t));
        /* initialize new entries to zero (meaning 'not computed') */
        for (size_t i = a_cache_size; i < new_size; ++i)
            a_cache[i] = 0;
        a_cache_size = new_size;
    }

    /* if already computed, return cached value */
    if (a_cache[n] != 0)
        return a_cache[n];

    seq_t modn = (seq_t)n;
    for (size_t k = 1;;++k) {
        ensure_prefix_sum(k);
        if (prefix_sums[k] % modn == 0) {
            a_cache[n] = (seq_t)k;
            return a_cache[n];
        }
    }
}
