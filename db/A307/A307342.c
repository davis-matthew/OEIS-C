/*
 * A307342  Products of four primes, except fourth powers of primes.
 * https://oeis.org/A307342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A307342: numbers with exactly four prime factors (counted with multiplicity)
   and with more than one distinct prime factor (bigomega=4 and omega>1) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_PRIME 2000000  /* sieve limit – more than enough for the first 10 000 terms */

static bool init_done = false;
static uint32_t *primes = NULL;
static size_t primes_cnt = 0;
static seq_t *terms = NULL;
static size_t terms_cap = 0;
static size_t terms_len = 0;
static uint64_t next_candidate = 2;

/* simple sieve of Eratosthenes */
static void init_sieve(void)
{
    bool *is_composite = (bool *)malloc((MAX_PRIME + 1) * sizeof(bool));
    if (!is_composite) exit(EXIT_FAILURE);
    memset(is_composite, 0, (MAX_PRIME + 1) * sizeof(bool));

    primes = (uint32_t *)malloc((MAX_PRIME / 10 + 1) * sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);

    for (uint32_t i = 2; i <= MAX_PRIME; ++i) {
        if (!is_composite[i]) {
            primes[primes_cnt++] = i;
            if ((uint64_t)i * i <= MAX_PRIME) {
                for (uint64_t j = (uint64_t)i * i; j <= MAX_PRIME; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);

    terms_cap = 128;
    terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    if (!terms) exit(EXIT_FAILURE);
}

/* add candidate x to the sequence if it satisfies the conditions */
static void add_if_valid(uint64_t x)
{
    uint64_t y = x;
    int omega = 0;      /* total number of prime factors counted with multiplicity */
    int distinct = 0;   /* number of distinct prime factors */
    uint64_t last_pf = 0;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > y) break;
        while (y % p == 0) {
            y /= p;
            ++omega;
            if (p != last_pf) {
                ++distinct;
                last_pf = p;
            }
        }
    }
    if (y > 1) {
        /* y is a prime larger than sqrt(original x) */
        ++omega;
        if (y != last_pf) {
            ++distinct;
            last_pf = y;
        }
    }

    if (omega == 4 && distinct > 1) {
        if (terms_len >= terms_cap) {
            terms_cap *= 2;
            terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            if (!terms) exit(EXIT_FAILURE);
        }
        terms[terms_len++] = (seq_t)x;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but harmless */

    if (!init_done) {
        init_sieve();
        init_done = true;
    }

    while (terms_len < (size_t)n) {
        add_if_valid(next_candidate++);
    }

    return terms[n - 1];
}
