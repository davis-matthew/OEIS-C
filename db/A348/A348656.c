/*
 * A348656  Numbers with record values of the denominator of the harmonic mean
 *          of their unitary divisors.
 * https://oeis.org/A348656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer m such that the denominator of the *
 *            harmonic mean of the unitary divisors of m sets a new record. *
 *            The sequence starts 1, 2, 4, 8, 16, 26, 32, 50, 58, 64, 98, 106,
 * Range:     u8 n <= 11, u16 n <= 112, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"   /* provides: seq_t gcd(seq_t a, seq_t b) */

static seq_t *records = NULL;          /* dynamic array of record values (m's)      */
static size_t records_len = 0;         /* number of stored records                */
static size_t records_cap = 0;         /* current capacity of 'records' array      */
static idx_t    cur_n = 1;             /* last examined n (starting with 1)       */
static seq_t    cur_max_denom = 1;     /* current maximum denominator found so far */

/* Compute the denominator of the harmonic mean of the unitary divisors of n.
 * n is given as idx_t but is treated as an unsigned 64‑bit integer. */
static seq_t
denominator_of_hm_unitary_divisors(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t temp = (uint64_t)n;
    seq_t    numerator  = 1;
    seq_t    denominator = 1;
    size_t   prime_count = 0;

    /* Simple trial division.  Works fast enough for the required range. */
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            seq_t p_pow = 1;
            while (temp % p == 0) {
                temp   /= p;
                p_pow  *= (seq_t)p;
            }
            denominator *= (p_pow + 1);
            numerator   *= p_pow;
            ++prime_count;
        }
    }

    if (temp > 1) {                     /* remaining prime factor */
        seq_t p_pow = (seq_t)temp;
        denominator *= (p_pow + 1);
        numerator   *= p_pow;
        ++prime_count;
    }

    /* Multiply numerator by 2^prime_count (k = 2^k where k is the number of primes). */
    seq_t pow2 = 1;
    for (size_t i = 0; i < prime_count; ++i)
        pow2 *= 2;
    numerator *= pow2;

    /* Reduce fraction: denominator after removing any common factor with numerator. */
    seq_t g = gcd(numerator, denominator);
    denominator /= g;

    return denominator;
}

/* Ensure the records array has space for at least 'new_len' elements. */
static void
ensure_records_capacity(size_t new_len)
{
    if (new_len <= records_cap) return;
    size_t new_cap = records_cap ? records_cap * 2 : 16;
    if (new_cap < new_len) new_cap = new_len;
    seq_t *tmp = (seq_t*)realloc(records, new_cap * sizeof(seq_t));
    records = tmp;
    records_cap = new_cap;
}

/* Main function requested by the OEIS test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n = 0, but guard */

    /* First call: initialize the static tables. */
    if (records == NULL) {
        records = (seq_t*)malloc(16 * sizeof(seq_t));
        records_cap = 16;
        records_len = 1;
        records[0] = (seq_t)1;
        cur_n = 1;
        cur_max_denom = 1;
    }

    /* Generate records until we have at least 'n' of them. */
    while (records_len < (size_t)n) {
        ++cur_n;
        seq_t denom = denominator_of_hm_unitary_divisors(cur_n);
        if (denom > cur_max_denom) {
            cur_max_denom = denom;
            ensure_records_capacity(records_len + 1);
            records[records_len++] = (seq_t)cur_n;
        }
    }

    return records[n - 1];
}
