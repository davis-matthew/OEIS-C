/*
 * A359157  Numbers whose odd part is squarefree and the number of prime
 *          factors (with multiplicity) is even.
 * https://oeis.org/A359157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth positive integer k such that * odd_part(k) is
 *            squarefree and Omega(k) (prime factors with multiplicity) * is
 *            even
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the standard headers required by this program */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Autonomous helper to generate primes up to a fixed limit. */
static uint32_t *primes = NULL;      /* array of primes */
static size_t prime_count = 0;       /* number of primes stored */

/* Build the list of all primes up to LIMIT once. */
static void init_primes(void)
{
    if (prime_count > 0) return;      /* already built */

    const uint32_t LIMIT = 200000;    /* sufficiently large for all test cases */
    uint8_t *is_comp = (uint8_t *)calloc(LIMIT + 1, 1); /* 0 = prime, 1 = composite */

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            /* store prime */
            uint32_t *tmp = (uint32_t *)realloc(primes, (prime_count + 1) * sizeof(uint32_t));
            if (!tmp) { free(is_comp); exit(EXIT_FAILURE); }
            primes = tmp;
            primes[prime_count++] = i;

            if ((uint64_t)i * (uint64_t)i <= LIMIT) {
                for (uint64_t j = (uint64_t)i * (uint64_t)i; j <= LIMIT; j += i)
                    is_comp[(size_t)j] = 1;
            }
        }
    }
    free(is_comp);
}

/* Test whether a number satisfies the condition:
 *  - its odd part is squarefree,
 *  - the total number of prime factors (with multiplicity) is even.
 */
static bool satisfies(uint64_t x)
{
    uint64_t m = x;
    int omega = 0;                     /* total prime multiplicity count */

    /* Remove all factors of 2 */
    while ((m & 1ULL) == 0ULL) {
        m >>= 1;
        ++omega;
    }

    /* Factor the remaining odd part */
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > m) break;

        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                ++exp;
                ++omega;
                m /= p;
            }
            if (exp > 1)            /* not squarefree */
                return false;
        }
    }

    if (m > 1)                        /* remaining prime factor */
        ++omega;

    return (omega % 2 == 0);           /* even total multiplicity? */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Lazy static state to keep the list of sequence terms. */
    static seq_t *seq         = NULL;
    static size_t seq_len     = 0;     /* number of terms already stored */
    static size_t seq_cap     = 0;     /* allocation capacity */
    static uint64_t current   = 1;     /* next integer to examine */

    /* Initialise prime list once. */
    init_primes();

    /* Allocate/extend the sequence array as needed. */
    if (!seq) {
        seq_cap = 1024;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them. */
    while (seq_len < n) {
        uint64_t cand = current++;
        if (satisfies(cand)) {         /* candidate satisfies the property */
            if (seq_len >= seq_cap) {
                seq_cap <<= 1;         /* double the capacity */
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)cand;
        }
    }

    return seq[n - 1];
}
