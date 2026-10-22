/*
 * A375010  a(n) = prime(n-1) - floor((prime(n-2) + prime(n-1) + prime(n)) /
 *          3).
 * https://oeis.org/A375010
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = prime(n-1) - floor((prime(n-2) + prime(n-1) +
 *            prime(n))/3)
 * Range:     i8 n <= 9885, i16 n <= 9885, i32 n <= 9885, i64 n <= 9885,
 *            i128 n <= 9885, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Dynamic prime table ---------------------------------------- */
static uint64_t *prime_list = NULL;   /* prime_list[0] = prime(1) = 2 */
static size_t   prime_count = 0;      /* number of primes stored          */
static size_t   prime_cap   = 0;      /* allocated capacity               */

/* Ensure at least k primes are present in prime_list. */
static void
ensure_primes(idx_t k)
{
    if (prime_count >= (size_t)k) return;

    if (prime_cap < (size_t)k) {                 /* grow capacity */
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        if (new_cap < (size_t)k) new_cap = (size_t)k;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(*tmp));
        if (!tmp) exit(EXIT_FAILURE);
        prime_list = tmp;
        prime_cap  = new_cap;
    }

    uint64_t candidate = (prime_count == 0) ? 2 : prime_list[prime_count - 1] + 1;

    while (prime_count < (size_t)k) {
        if (candidate == 2) {                  /* handle the first prime */
            prime_list[prime_count++] = 2;
            candidate = 3;
            continue;
        }
        if ((candidate & 1ULL) == 0) {          /* skip even numbers */
            candidate += 1;
            continue;
        }

        int is_prime = 1;
        /* trial division by known primes up to sqrt(candidate) */
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) prime_list[prime_count++] = candidate;
        candidate += 1;
    }
}

/* Return a(n) for n >= 3. Behaviour for n < 3 is unspecified; returns 0. */
seq_t a(idx_t n)
{
    if (n < 3) return 0;

    /* Ensure we have at least n primes (indices 1..n). */
    ensure_primes(n);

    /* Indices in C array start at 0, prime(1) is at prime_list[0] */
    uint64_t p0 = prime_list[(size_t)n - 3];   /* prime(n-2) */
    uint64_t p1 = prime_list[(size_t)n - 2];   /* prime(n-1) */
    uint64_t p2 = prime_list[(size_t)n - 1];   /* prime(n)   */

    uint64_t avg = (p0 + p1 + p2) / 3;         /* floor division */
    seq_t diff   = (seq_t)p1 - (seq_t)avg;     /* result in seq_t */

    return diff;
}
