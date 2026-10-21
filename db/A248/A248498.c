/*
 * A248498  The smallest number of consecutive primes beginning with
 *          prime(n+1) added to prime(n) necessary to produce another prime.
 * https://oeis.org/A248498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1365, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A248498:
   a(n) = the smallest number k such that prime(n) +
          prime(n+1)+...+prime(n+k) is prime.
   The function uses a simple prime generator and checks sums
   until a prime is found.  Memoisation is applied via a
   static prime list, so calls do not depend on order. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"
#include "../utils/isPrimeGeneric.h"

static uint64_t *prime_list = NULL;   /* list of primes, index 0 = prime(1) */
static size_t  prime_cnt   = 0;        /* number of primes stored      */
static uint64_t next_candidate = 2;    /* next integer to test */

/* Ensure prime_list contains at least (index+1) primes (indices 0..index). */
static void ensure_prime_count(size_t index)
{
    if (prime_cnt > index)
        return;

    size_t new_cap = prime_cnt ? prime_cnt << 1U : 16U;
    if (new_cap <= index + 1U)
        new_cap = index + 1U;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp)
        abort();                      // out‑of‑memory
    prime_list = tmp;

    while (prime_cnt <= index) {
        if (next_candidate == 2) {
            prime_list[prime_cnt++] = 2;
            next_candidate = 3;
            continue;
        }
        if ((next_candidate & 1U) == 0) {
            ++next_candidate;
            continue;
        }
        if (is_prime_small(next_candidate))
            prime_list[prime_cnt++] = next_candidate;
        next_candidate += 2U;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)           // undefined for n < 1, but guard
        return (seq_t)0;

    /* prime(n) is at index n-1 */
    ensure_prime_count((size_t)n - 1U);
    seq_t sum = (seq_t)prime_list[n - 1U];
    seq_t k   = 0;                 /* number of primes added so far */
    size_t idx = (size_t)n;        /* index of prime(n+1) in prime_list */

    for (;;) {
        ensure_prime_count(idx);
        sum += (seq_t)prime_list[idx];
        ++k;
        if (isPrimeGeneric(sum))
            return k;
        ++idx;
    }
}
