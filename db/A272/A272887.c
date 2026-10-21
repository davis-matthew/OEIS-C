/*
 * A272887  Number of ways to write prime(n) as (4*x + 2)*y + 4*x + 1 where x
 *          and y are nonnegative integers.
 * https://oeis.org/A272887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n = 1; for n ≥ 2, a(n) = number of odd divisors of
 *            (1 + prime(n)) (offset 1 – a(1) is the first listed term). The
 *            main routine computes the result by sieving primes up to a
 *            sufficient bound and then factoring the odd part of 1+prime(n)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ------------------------------------------------------------------
   Prime sieve – dynamic, re‑initialise if a larger bound is needed
   ------------------------------------------------------------------ */
static uint32_t *prime_array = NULL;   /* primes in order, 0‑based */
static size_t    prime_count = 0;      /* number of primes stored */
static idx_t     prime_limit  = 0;      /* largest number sieved up to */

/* Allocate or rebuild the prime list up to at least 'limit' */
static void init_primes_up_to(idx_t limit)
{
    if (prime_array != NULL && prime_limit >= limit)
        return;   /* already sufficient */

    /* free old array */
    if (prime_array)
        free(prime_array);

    /* allocate a boolean array for composites (0 = prime candidate) */
    size_t sz = (size_t)limit + 1;
    bool *is_comp = (bool*)malloc(sz * sizeof(bool));
    if (!is_comp)
        return;              /* out of memory – caller will fail */
    memset(is_comp, 0, sz * sizeof(bool));

    /* capacity for the primes array – grow as needed */
    size_t cap = limit / 10 + 10;
    prime_array = (uint32_t*)malloc(cap * sizeof(uint32_t));
    if (!prime_array) {
        free(is_comp);
        return;
    }
    prime_count = 0;

    for (uint64_t i = 2; i <= (uint64_t)limit; ++i) {
        if (!is_comp[(size_t)i]) {
            if (prime_count == cap) {          /* double the storage */
                cap *= 2;
                uint32_t *tmp = (uint32_t*)realloc(prime_array, cap * sizeof(uint32_t));
                if (!tmp)            /* allocation failed */
                    break;
                prime_array = tmp;
            }
            prime_array[prime_count++] = (uint32_t)i;

            if (i * i <= (uint64_t)limit) {
                /* mark multiples of i starting at i*i */
                for (uint64_t j = i * i; j <= (uint64_t)limit; j += i)
                    is_comp[(size_t)j] = true;
            }
        }
    }
    free(is_comp);
    prime_limit = limit;
}

/* ------------------------------------------------------------------
   Main sequence function
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Special case for the first term (offset 1) */
    if (n == 1)
        return (seq_t)0;

    /* Ensure we have enough primes: start with a comfortable bound */
    if (prime_array == NULL)
        init_primes_up_to(200000);   /* covers prime(10000) ≈ 104729 */

    /* If the requested index exceeds current primes, grow the sieve */
    while (n > prime_count) {
        init_primes_up_to(prime_limit * 2 + 1);
    }

    /* The nth prime (1‑based index) */
    uint64_t p = (uint64_t)prime_array[(size_t)(n - 1)];

    /* Compute 1 + p */
    uint64_t v = p + 1ULL;

    /* Strip away factors of 2; we only need the odd part */
    while ((v & 1ULL) == 0ULL)
        v >>= 1ULL;

    /* Count odd divisors of v by prime factorisation */
    seq_t result = (seq_t)1;

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t q = prime_array[i];
        uint64_t q2 = (uint64_t)q * (uint64_t)q;
        if (q2 > v)
            break;

        if (v % (uint64_t)q == 0ULL) {
            uint32_t exp = 0;
            while (v % (uint64_t)q == 0ULL) {
                v /= (uint64_t)q;
                ++exp;
            }
            result *= (seq_t)(exp + 1);
        }
    }

    /* If anything remains, it's an odd prime factor with exponent 1 */
    if (v > 1ULL)
        result *= (seq_t)2;

    return result;
}
