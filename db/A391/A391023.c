/*
 * A391023  Numbers with more than 1 distinct prime factor and whose largest
 *          prime factor exponent is 2.
 * https://oeis.org/A391023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS A391023: numbers with more than one distinct
 * prime factor and whose largest prime factor exponent is exactly 2. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"   /* build_primes_upto(idx_t, uint64_t**, size_t*) */
#include "../utils/int_sqrt.h"            /* int_sqrt(idx_t) returns ⌊√x⌋ */

static uint64_t *prime_list = NULL;   /* primes ≤ prime_limit */
static size_t      prime_count = 0;
static uint64_t    prime_limit = 0;
static uint64_t    candidate = 2;     /* next integer to test */

static seq_t  *seq_arr = NULL;        /* array of found sequence terms */
static size_t  seq_len = 0;           /* number of terms computed so far */
static size_t  seq_cap = 0;           /* current allocation capacity */

/* Ensure that prime_list contains all primes ≤ limit (inclusive).  If the
 * current list is too short, free it and rebuild. */
static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= prime_limit) return;   /* already sufficient */

    if (prime_list != NULL) free(prime_list);

    build_primes_upto(limit, &prime_list, &prime_count);
    prime_limit = limit;
}

/* Return true if n satisfies the definition of A391023. */
static bool qualifies(uint64_t n)
{
    if (n < 12) return false;   /* 12 is the smallest term in the sequence */

    uint64_t m = n;
    int distinct = 0;
    int max_exp  = 0;

    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= m; ++i)
    {
        uint64_t p = prime_list[i];
        if (m % p == 0) {
            ++distinct;
            int exp = 0;
            while (m % p == 0) { m /= p; ++exp; }
            if (exp > max_exp) max_exp = exp;
        }
    }

    if (m > 1) {            /* remaining prime factor > √original n */
        ++distinct;
        if (1 > max_exp) max_exp = 1;
    }

    /* At least two distinct primes, and the largest exponent must be exactly 2. */
    return (distinct >= 2) && (max_exp == 2);
}

/* Generate the next qualifying number and append it to seq_arr. */
static void generate_next(void)
{
    while (true) {
        uint64_t limit = (uint64_t)int_sqrt(candidate) + 1;
        if (limit > prime_limit) ensure_primes_upto(limit);

        if (qualifies(candidate)) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 8;
                seq_t *tmp = (seq_t*)realloc(seq_arr, new_cap * sizeof(seq_t));
                if (!tmp) { seq_arr = NULL; seq_cap = seq_len = 0; return; }
                seq_arr = tmp; seq_cap = new_cap;
            }
            seq_arr[seq_len++] = (seq_t)candidate;
            ++candidate;
            return;
        }
        ++candidate;
    }
}

/* Public sequence function: a(n) returns the nth term (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined, but harmless */

    while (seq_len < (size_t)n) generate_next();
    return seq_arr[n - 1];
}
