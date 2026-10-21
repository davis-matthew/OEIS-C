/*
 * A256736  Number of composites lying between successive pairs of primes,
 *          beginning with pair (3,5). Bisection of A046933.
 * https://oeis.org/A256736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of composites between the (2n‑1)th and 2nth
 *            primes, i.e. a(n) = primes[2n] – primes[2n‑1] – 1 Offset: 1
 *            (a(1) corresponds to the pair (3,5) )
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *prime_list   = NULL;   /* stored as seq_t */
static size_t prime_count  = 0;      /* number of primes already stored */
static size_t prime_cap    = 0;      /* current allocated capacity */
static idx_t  next_candidate = 2;    /* next integer to test for primality */

/* Ensure that the prime array has at least 'min_cap' elements */
static void grow_prime_cap(size_t min_cap)
{
    if (prime_cap >= min_cap)
        return;
    size_t new_cap = prime_cap ? prime_cap << 1U : 32;
    while (new_cap < min_cap)
        new_cap <<= 1U;
    seq_t *tmp = (seq_t *)realloc(prime_list, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);      /* memory allocation failed */
    prime_list = tmp;
    prime_cap  = new_cap;
}

/* Generate primes until at least 'max_index' (0‑based) is stored. */
static void ensure_primes_up_to_index(size_t max_index)
{
    if (prime_count > max_index)
        return;
    while (prime_count <= max_index) {
        /* Handle the very first prime (2) specially. */
        if (next_candidate == 2) {
            grow_prime_cap(prime_count + 1);
            prime_list[prime_count++] = (seq_t)2;
            next_candidate = 3;
            continue;
        }

        /* Skip even candidates. */
        if ((next_candidate & 1U) == 0U)
            ++next_candidate;

        /* Find the next prime by trial division using the provided helper. */
        while (!is_prime(next_candidate))
            next_candidate += 2U;

        grow_prime_cap(prime_count + 1);
        prime_list[prime_count++] = (seq_t)next_candidate;
        ++next_candidate;   /* move to the next integer after this prime */
    }
}

seq_t a(idx_t n)
{
    if (n < 1U)
        return (seq_t)0;          /* undefined for n < 1 */

    /* We need the primes at indices 2n‑1 and 2n (0‑based). */
    size_t required_index = 2U * (size_t)n;   /* maximum index needed */
    ensure_primes_up_to_index(required_index);

    seq_t p_prev = prime_list[(size_t)(2U * n - 1U)];
    seq_t p_next = prime_list[(size_t)(2U * n)];
    return p_next - p_prev - (seq_t)1;
}
