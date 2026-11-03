/*
 * A276307  Primes p such that d(p*(2p+1)) = 16 where d(n) is the number of
 *          divisors of n (A000005).
 * https://oeis.org/A276307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1750, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9798 (2026-11-03)
 * Helpers:   is_prime, mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A276307
   a(n) = the n-th prime p such that d(p*(2p+1)) = 16
   (equivalently, d(2p+1) = 8)
   The function keeps an internal cache, so repeated calls are fast.
*/
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/num_divisors.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

extern seq_t a(idx_t n)
{
    /* static cache variables */
    static seq_t *cache = NULL;   /* dynamic array of primes */
    static size_t cache_len = 0;  /* number of stored terms   */
    static size_t cache_cap = 0;  /* allocated capacity       */
    static idx_t next_candidate = 3;  /* next odd number to test */

    if (n == 0)          /* offset 1: a(1) is first term, ignore n==0 */
        return 0;

    /* generate terms until we have at least n entries */
    while (cache_len < (size_t)n) {
        if (is_prime(next_candidate)) {            /* test primality of p */
            /* compute 2p+1 safely in uint64_t; numbers in the
               relevant range for n up to 10000 fit in 64 bits */
            uint64_t fourp = 2ULL * (uint64_t)next_candidate + 1ULL;
            if (num_divisors(fourp) == 8U) {   /* d(2p+1) == 8?   */
                append_candidate(&cache, &cache_len, &cache_cap,
                                 (seq_t)next_candidate);
            }
        }
        next_candidate += 2;   /* skip even numbers */
    }
    return cache[(size_t)n - 1];
}
