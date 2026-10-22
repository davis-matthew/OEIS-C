/*
 * A374845  The numbers p or 2p with p prime and p = 3 mod 4, in ascending
 *          order.
 * https://oeis.org/A374845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A374845 – merge of primes ≡ 3 (mod 4) and their doubles
 * Range:     u8 n <= 44, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the n‑th element (1‑based) of that sequence. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"

/* Cache for the sequence terms and for the primes ≡ 3 (mod 4). */
static seq_t *seq_cache = NULL;   /* cached values of the sequence */
static size_t seq_len  = 0;       /* number of cached terms */
static size_t seq_cap  = 0;       /* capacity of seq_cache */

static seq_t *prime3 = NULL;     /* primes ≡ 3 (mod 4) */
static size_t prime_len = 0;     /* number of cached primes */
static size_t prime_cap = 0;     /* capacity of prime3 */

static uint64_t next_candidate = 3;   /* next odd integer to test for primality */

static size_t merge_i_prime  = 0;  /* next index to use from prime3 */
static size_t merge_i_double = 0;  /* next index to use from prime3 when doubling */

/* Ensure that seq_cache contains at least n terms. */
static void ensure_sequence(idx_t n)
{
    while (seq_len < (size_t)n) {
        /* Need enough primes for the smallest merge index. */
        size_t need = merge_i_prime > merge_i_double ? merge_i_prime : merge_i_double;
        while (prime_len <= need) {
            /* Find the next prime ≡ 3 (mod 4). */
            while (!(is_prime(next_candidate) && (next_candidate % 4 == 3))) {
                next_candidate += 2;         /* only odd numbers */
            }

            /* Grow the prime list if necessary. */
            if (prime_len == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(prime3, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);           /* allocation failure */
                prime3 = tmp;
                prime_cap = new_cap;
            }
            prime3[prime_len++] = (seq_t)next_candidate;
            next_candidate += 2;             /* next odd candidate */
        }

        /* Merge the next smallest value from prime3 or its double. */
        seq_t val1 = prime3[merge_i_prime];
        seq_t val2 = prime3[merge_i_double] * (seq_t)2; /* double the prime */

        seq_t next_val;
        if (val1 <= val2) {
            next_val = val1;
            ++merge_i_prime;
        } else {
            next_val = val2;
            ++merge_i_double;
        }

        /* Grow the sequence cache if necessary. */
        if (seq_len == seq_cap) {
            size_t new_cap = seq_cap ? seq_cap << 1 : 16;
            seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);           /* allocation failure */
            seq_cache = tmp;
            seq_cap = new_cap;
        }
        seq_cache[seq_len++] = next_val;
    }
}

/* External function returning the n-th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* offset is 1; behaviour for n<1 is arbitrary */
    if (n <= seq_len) return seq_cache[(size_t)n - 1];
    ensure_sequence(n);
    return seq_cache[(size_t)n - 1];
}
