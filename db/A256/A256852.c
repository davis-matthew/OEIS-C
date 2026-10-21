/*
 * A256852  Number of ways to write prime(n) = a^2 + b^4.
 * https://oeis.org/A256852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to prime(n) = a^2 + b^4 where a,b are
 *            non‑negative integers. The value returned is at most 2 as proved
 *            in the OEIS entry. The function memoizes the list of primes
 *            needed for all n up to the maximum seen. It works for n up to at
 *            least 10 000 within the time limit
 * Range:     u8 n <= 18, u16 n <= 1771, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t x); */

static uint64_t *prime_list = NULL;   /* primes in ascending order */
static size_t    prime_count = 0;     /* number of primes stored */

/* ------------------------------------------------------------------
   Simple sieve of Eratosthenes.
   limit must be at least 2.  The returned array is malloc‑allocated
   and should be freed by the caller (we keep the most recent one
   between calls, so we free the previous one if the new limit is
   larger).
------------------------------------------------------------------ */
static void generate_primes_up_to(idx_t limit)
{
    if (limit < 2) return;

    /* allocate array for primes; the prime number theorem gives
       approximately limit / log(limit) primes.  For limit <= 1e6
       this is far less than 1.2e5, so the following allocation is
       safe. */
    size_t alloc_cap = (limit / 10) + 10;
    uint64_t *new_primes = (uint64_t *)malloc(alloc_cap * sizeof(uint64_t));
    if (new_primes == NULL) abort();

    /* sieve */
    uint8_t *is_prime = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    if (is_prime == NULL) abort();
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) is_prime[j] = 0;
        }
    }

    /* collect primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (cnt >= alloc_cap) {   /* grow if needed */
                alloc_cap *= 2;
                new_primes = (uint64_t *)realloc(new_primes,
                                                 alloc_cap * sizeof(uint64_t));
                if (new_primes == NULL) abort();
            }
            new_primes[cnt++] = (uint64_t)i;
        }
    }

    free(is_prime);

    /* replace global list */
    if (prime_list != NULL) free(prime_list);
    prime_list  = new_primes;
    prime_count = cnt;
}

/* ------------------------------------------------------------------
   Ensure that we have at least n primes available.
------------------------------------------------------------------ */
static void ensure_primes(idx_t n)
{
    if (n <= prime_count) return;          /* already have enough */

    /* For n up to 10000 the 10 000th prime is 104 729.
       We use a simple growth strategy: start with 200 000,
       and double if necessary. */
    idx_t limit = 200000;
    while (prime_count < n) {
        generate_primes_up_to(limit);
        if (prime_count >= n) break;
        limit *= 2;                       /* increase sieve size */
    }
}

/* ------------------------------------------------------------------
   Count representations of the nth prime as a^2 + b^4.
------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* undefined for n<1, harmless */
    ensure_primes(n);
    seq_t p = (seq_t)prime_list[n - 1]; /* nth prime (1‑based) */

    seq_t count = 0;
    for (idx_t b = 0;; ++b) {
        seq_t b_seq = (seq_t)b;
        seq_t b2 = b_seq * b_seq;
        seq_t b4 = b2 * b2;
        if (b4 > p) break;
        seq_t a2 = p - b4;
        seq_t a = floor_sqrt(a2);
        if (a * a == a2) ++count;
    }
    return count;
}
