/*
 * A268377  Numbers n such that any prime factor of the form 4k+1 has even
 *          multiplicity.
 * https://oeis.org/A268377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 138, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A268377: numbers n such that every prime factor of the
   form 4k+1 appears with even exponent.  The implementation
   generates a prime list once and tests each candidate.  It works
   for any seq_t and idx_t supplied by the framework. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Prime table built lazily on first use.  */
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;

/* Build a list of primes up to LIMIT.  The limit is set high
   enough to cover the first 10 000 terms of the sequence.  */
static void build_primes(void)
{
    if (prime_list != NULL)
        return;        /* already built */

    /* 200 000 is a generous upper bound for the candidates needed
       to reach the first 10 000 terms.  */
    const size_t LIMIT = 200000;

    bool *marked = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (marked == NULL)
        return;        /* allocation failure – rare in tests */

    size_t cap = LIMIT / 8 + 1;
    prime_list = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (prime_list == NULL) {
        free(marked);
        return;
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!marked[i]) {
            if (cnt == cap) {
                cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, cap * sizeof(uint64_t));
                if (!tmp) {          /* allocation failure */
                    free(marked);
                    free(prime_list);
                    prime_list = NULL;
                    return;
                }
                prime_list = tmp;
            }
            prime_list[cnt++] = i;

            if (i * i <= LIMIT) {
                for (size_t j = i * i; j <= LIMIT; j += i)
                    marked[j] = true;
            }
        }
    }
    free(marked);
    prime_cnt = cnt;
}

/* Test whether x satisfies the OEIS property:
   each prime factor of the form 4k+1 appears an even number of times. */
static bool ok_value(uint64_t x)
{
    if (x <= 1)
        return true;

    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= x; ++i) {
        uint64_t p = prime_list[i];
        if (x % p == 0) {
            unsigned exp = 0;
            do {
                x /= p;
                ++exp;
            } while (x % p == 0);

            if ((p & 3ULL) == 1 && (exp & 1U))
                return false;          /* odd exponent for a 4k+1 prime */
        }
    }
    /* x might be a remaining prime larger than sqrt(original x). */
    if (x > 1 && (x & 3ULL) == 1)
        return false;

    return true;
}

/* External interface required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;        /* the sequence is 1‑based */

    build_primes();    /* ensure primes are ready */

    uint64_t count = 0;
    uint64_t cand   = 1;

    while (count < n) {
        if (ok_value(cand))
            ++count;
        ++cand;
    }

    return (seq_t)(cand - 1);
}
