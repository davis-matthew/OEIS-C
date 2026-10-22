/*
 * A363597  Union of prime powers and numbers that are not squarefree.
 * https://oeis.org/A363597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Sequence A363597 consists of integers that are prime powers, *
 *            or composites that are not squarefree (i.e. all integers except
 *            * composite squarefree numbers). This implementation enumerates
 *            numbers * and tests membership using trial division by primes
 * Range:     u8 n <= 153, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _int_sqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_int_sqrt_u64.h"

/* Dynamic array of primes, rebuilt each time we need a larger bound. */
static uint32_t *primes = NULL;        /* primes array */
static size_t primes_len = 0;          /* number of primes stored */
static size_t primes_cap = 0;          /* allocated capacity */
static uint64_t primes_limit = 0;      /* largest prime already computed */

/* Rebuild the list of primes up to 'limit' (inclusive). */
static void _ensure_primes(uint64_t limit)
{
    if (limit <= primes_limit) return;

    uint64_t new_limit = limit;
    bool *is_comp = (bool *)malloc((new_limit + 1) * sizeof(bool));
    if (!is_comp) abort();
    for (uint64_t i = 0; i <= new_limit; ++i) is_comp[i] = false;

    primes_len = 0;  /* start fresh */
    for (uint64_t i = 2; i <= new_limit; ++i) {
        if (!is_comp[i]) {
            if (primes_len == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!primes) abort();
            }
            primes[primes_len++] = (uint32_t)i;
            if (i > new_limit / i) continue;
            for (uint64_t j = i * i; j <= new_limit; j += i) {
                is_comp[j] = true;
            }
        }
    }
    free(is_comp);
    primes_limit = new_limit;
}

/* Test whether x belongs to the sequence. */
static bool _in_sequence(uint64_t x)
{
    if (x == 1) return true;        /* empty product */

    uint64_t limit = _int_sqrt_u64(x) + 1;
    _ensure_primes(limit);

    uint64_t temp = x;
    int prime_cnt = 0;

    for (size_t i = 0; i < primes_len && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            int e = 0;
            ++prime_cnt;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (e > 1) return true;   /* has a repeated prime factor */
        }
    }
    if (temp > 1) ++prime_cnt;       /* remaining prime factor */

    if (prime_cnt >= 2) return false;  /* composite squarefree */
    return true;      /* prime or prime power (handled above) */
}

/* Dynamic array of computed sequence values (seq_t).  */
static seq_t *seq_arr = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static uint64_t next_candidate = 1;   /* next integer to test */

/* Main function required by the harness.  */
seq_t a(idx_t n)
{
    /* n >= 1 as per the OEIS offset.  */
    while (seq_len < (size_t)n) {
        uint64_t cur = next_candidate++;
        if (_in_sequence(cur)) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 8;
                seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
                if (!seq_arr) abort();
            }
            seq_arr[seq_len++] = (seq_t)cur;
        }
    }
    return seq_arr[n - 1];
}
