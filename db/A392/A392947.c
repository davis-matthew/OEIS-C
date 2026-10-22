/*
 * A392947  Primes p such that the multiplicative order of -2 modulo p is a
 *          multiple of 4.
 * https://oeis.org/A392947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 2734, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, mod_pow64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A392947: primes p such that the multiplicative order of -2 modulo p
   is a multiple of 4.
   The function increments through primes, tests the order of -2 modulo each,
   and keeps a static list of the primes that satisfy the property. */

/* 1) needed includes
   2) use the supplied utility functions is_prime and mod_pow64
   3) implementation uses a static array of uint64_t for the primes;
      the result is cast to seq_t when returned. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow64.h"

/* Return true iff the multiplicative order of -2 modulo prime p is a multiple of 4. */
static bool order_minus2_ok(uint64_t p)
{
    /* (-2) mod p */
    uint64_t base = p - 2;
    /* start with p-1, then reduce by prime factors */
    uint64_t ord  = p - 1;
    uint64_t temp = ord;

    for (uint64_t q = 2; q * q <= temp; ++q) {
        if (temp % q == 0) {
            while (ord % q == 0 && mod_pow64(base, ord / q, p) == 1) {
                ord /= q;                      /* remove factor q from order */
            }
            while (temp % q == 0) temp /= q;  /* skip factor q in temp */
        }
    }
    if (temp > 1) {                           /* remaining prime factor */
        while (ord % temp == 0 &&
               mod_pow64(base, ord / temp, p) == 1) {
            ord /= temp;
        }
    }
    return (ord % 4 == 0);
}

/* Return the nth element of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    static uint64_t *prime_list = NULL;  /* array of primes satisfying the condition */
    static size_t    list_cap   = 0;
    static size_t    list_size  = 0;
    static uint64_t  candidate  = 3;     /* next odd candidate to test */

    if (n == (idx_t)0) return 0;         /* undefined for 0, safe guard */

    while (list_size < (size_t)n) {
        /* Find the next prime that satisfies the condition */
        for (;;) {
            candidate = (!candidate) ? 3 : candidate; /* ensure at least 3 */
            if ((candidate & 1ULL) == 0) {            /* skip even numbers */
                ++candidate;
                continue;
            }
            if (!is_prime(candidate)) {                /* candidate is not prime */
                ++candidate;
                continue;
            }
            if (order_minus2_ok(candidate)) {          /* prime satisfies condition */
                /* store prime */
                if (list_size >= list_cap) {
                    size_t new_cap = list_cap ? list_cap * 2 : 128;
                    uint64_t *tmp =
                        (uint64_t *)realloc(prime_list,
                                            new_cap * sizeof(uint64_t));
                    if (!tmp) return 0;                /* allocation failure */
                    prime_list = tmp;
                    list_cap   = new_cap;
                }
                prime_list[list_size++] = candidate;
                ++candidate;                            /* continue after this prime */
                break;
            }
            ++candidate;                                  /* try next one */
        }
    }
    /* cast the stored uint64_t to the required seq_t type */
    return (seq_t)prime_list[n - 1];
}
