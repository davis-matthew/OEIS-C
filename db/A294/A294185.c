/*
 * A294185  Number of distinct lesser twin primes which are in Goldbach
 *          partitions of 2n.
 * https://oeis.org/A294185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of distinct lesser twin primes that appear in any Goldbach
   partition of the even number 2*n.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ----- Sieve helpers (static) ----- */
static bool *is_prime = NULL;          /* sieve table (0..limit) */
static idx_t sieve_limit = 0;          /* maximum value for which sieve is valid */

/* Rebuild the sieve up to new_limit (reallocating as needed). */
static void extend_sieve(idx_t new_limit)
{
    if (new_limit <= sieve_limit) return;

    /* Allocate or enlarge sieve array and initialise to true. */
    bool *new_table = (bool *)realloc(is_prime, sizeof(bool) * (new_limit + 1));
    if (!new_table) exit(1);
    is_prime = new_table;
    for (idx_t i = 0; i <= new_limit; ++i) is_prime[i] = true;
    if (new_limit >= 0) is_prime[0] = false;
    if (new_limit >= 1) is_prime[1] = false;

    /* Classic Sieve of Eratosthenes. */
    for (idx_t p = 2; p * p <= new_limit; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= new_limit; j += p)
                is_prime[j] = false;
        }
    }

    sieve_limit = new_limit;
}

/* ----- Main sequence function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* safety for n=0; OEIS offset is 1 */

    idx_t even = n * 2;
    extend_sieve(even);                 /* generate primes up to 2n */

    /* Allocate boolean array to deduplicate counted primes. */
    size_t seen_size = (size_t)(even + 1);
    bool *seen = (bool *)calloc(seen_size, sizeof(bool));
    if (!seen) exit(1);

    seq_t count = (seq_t)0;

    /* Loop over all odd primes p up to even. */
    for (idx_t p = 3; p <= even; p += 2) {
        if (!is_prime[p]) continue;
        idx_t q = even - p;
        if (q < 2 || q > sieve_limit || !is_prime[q]) continue;

        /* Check p itself. */
        if (p + 2 <= sieve_limit && is_prime[p + 2]) {
            if (!seen[p]) {
                ++count;
                seen[p] = true;
            }
        }

        /* Check q if distinct from p. */
        if (p != q && q + 2 <= sieve_limit && is_prime[q + 2]) {
            if (!seen[q]) {
                ++count;
                seen[q] = true;
            }
        }
    }

    free(seen);
    return count;
}
