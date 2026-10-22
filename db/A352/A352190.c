/*
 * A352190  Indices of records in A352188.
 * https://oeis.org/A352190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 662, u32 n <= 662, u64 n <= 662,
 *            u128 n <= 662, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 1 if n = 1; otherwise a(n) is the (n-1)-th prime */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Static state for prime generation */
static seq_t *prime_list = NULL;
static size_t prime_cap  = 0;
static size_t prime_len  = 0;
static seq_t  next_candidate = 3;

/* Ensure at least 'count' primes are available in prime_list.
   count is 1-based: count=1 means the first prime (2) should be stored. */
static void
ensure_primes_count(idx_t count)
{
    if (prime_list == NULL) {
        prime_cap = 64;
        prime_list = (seq_t *)malloc(prime_cap * sizeof(seq_t));
        prime_list[0] = (seq_t)2;   /* first prime */
        prime_len   = 1;
    }

    /* Generate more primes until we have at least 'count' entries */
    while ((idx_t)prime_len < count) {
        seq_t candidate = next_candidate;
        bool is_prime = true;

        /* Test divisibility by known primes up to sqrt(candidate) */
        for (size_t i = 0; i < prime_len; ++i) {
            seq_t p = prime_list[i];
            /* Compute p * p in 128-bit to avoid overflow */
            if ((unsigned __int128)p * (unsigned __int128)p > (unsigned __int128)candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Grow array if needed */
            if (prime_len >= prime_cap) {
                prime_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(prime_list, prime_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failed; abort to avoid undefined behaviour */
                    exit(1);
                }
                prime_list = tmp;
            }
            prime_list[prime_len++] = candidate;
        }
        next_candidate += 2;  /* only odd candidates after 2 */
    }
}

/* Public function to return a(n) */
seq_t
a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Need the (n-1)-th prime: primes are stored 0-indexed. */
    idx_t prime_index = n - 2;          /* zero-based */
    idx_t required    = prime_index + 1;/* number of primes needed */

    ensure_primes_count(required);
    return prime_list[prime_index];
}
