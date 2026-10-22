/*
 * A349573  a(n) = A048673(n) - n, where A048673(n) = (A003961(n)+1) / 2, and
 *          A003961(n) shifts the prime factorization of n one step towards
 *          larger primes.
 * https://oeis.org/A349573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = (A003961(n)+1)/2 - n, where A003961 shifts each * prime
 *            factor of n to the next prime
 * Range:     i8 n <= 23, i16 n <= 1535, i32 n <= 9655, i64 n <= 9655,
 *            i128 n <= 9655, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static bool primes_ready  = false;

/* Ensure we have a list of primes up to at least 20000. 2e4 is more than enough
 * for the next prime after any factor ≤ 10000. */
static void ensure_primes(void) {
    if (prime_list != NULL) return;        /* already built */
    build_primes_upto((idx_t)20000, &prime_list, &prime_cnt);
    primes_ready = true;
}

/* Find the index of a prime 'p' in prime_list.  prime_list is sorted. */
static size_t prime_index(uint64_t p) {
    for (size_t i = 0; i < prime_cnt; ++i) {
        if (prime_list[i] == p) return i;
    }
    /* Should never happen for valid inputs. */
    return prime_cnt;  /* sentinel, caller will guard */
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* no defined value below offset */
    ensure_primes();

    idx_t   rem   = n;
    seq_t   prod  = (seq_t)1;              /* product of shifted primes */

    /* Trial division using precomputed primes up to sqrt(rem). */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * (uint64_t)p > (uint64_t)rem) break;

        if (rem % p == 0) {
            int exp = 0;
            while (rem % p == 0) {
                rem /= p;
                ++exp;
            }
            /* Next prime after p is stored at i+1. Since primes were built
             * to a large limit, the next prime surely exists. */
            uint64_t next = prime_list[i + 1];
            /* Multiply prod by next^exp */
            for (int j = 0; j < exp; ++j) {
                prod *= (seq_t)next;
            }
        }
    }

    /* If a factor larger than sqrt(n) remains, it is a prime. */
    if (rem > 1) {
        size_t idx = prime_index(rem);
        uint64_t next = prime_list[idx + 1];  /* guaranteed to exist */
        prod *= (seq_t)next;
    }

    /* Now prod = A003961(n). Compute a(n). */
    seq_t half = (prod + (seq_t)1) / (seq_t)2;
    return half - (seq_t)n;
}
