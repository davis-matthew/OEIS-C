/*
 * A390027  Record denominators of (p1-1)/(p2-1), where p1 < p2 are
 *          consecutive primes.
 * https://oeis.org/A390027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the record denominator of (p1-1)/(p2-1) for consecutive
 *            primes p1 < p2. The denominator of the reduced fraction is
 *            (p2-1)/gcd(p1-1,p2-1); a record occurs when this denominator
 *            exceeds all previous ones
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation keeps the computed primes and records
    in static arrays so that calls are independent and fast. */
/*  It uses only integer arithmetic; gcd is computed by a
    small Euclidean routine working on 64‑bit unsigned numbers
    because all values are < 2^64 for the requested limits. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd_uint64.h"

/*--- Prime generation ---------------------------------------------------*/

static uint64_t *prime_list   = NULL;   /* dynamic array of found primes   */
static size_t   prime_cap    = 0;       /* current capacity                */
static size_t   prime_cnt    = 0;       /* number of primes stored         */
static uint64_t last_candidate = 3;     /* next odd candidate to test      */

/* Ensure the prime list can hold at least 'need' primes.   */
static void ensure_prime_cap(size_t need)
{
    if (prime_cap >= need) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_list = tmp;
    prime_cap  = new_cap;
}

/* Test whether a candidate is prime using the primes found so far. */
static bool is_prime_candidate(uint64_t candidate)
{
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > candidate / p) break;  /* p*p > candidate */
        if (candidate % p == 0) return false;
    }
    return true;
}

/* Return the first prime strictly greater than 'start'.
   For start < 2 it returns 2, for start == 2 it returns 3. */
static uint64_t next_prime(uint64_t start)
{
    if (start < 2) start = 2;
    uint64_t candidate = start + 1;
    if ((candidate & 1U) == 0) ++candidate;  /* make odd */
    if (candidate < last_candidate) candidate = last_candidate;
    for (;;) {
        if (is_prime_candidate(candidate)) {
            ensure_prime_cap(prime_cnt + 1);
            prime_list[prime_cnt++] = candidate;
            last_candidate = candidate + 2;
            return candidate;
        }
        candidate += 2;
    }
}

/*--- Greatest common divisor for 64‑bit integers -------------------------*/

/*--- Record denominators ----------------------------------------------*/

static seq_t *rec     = NULL;   /* array of recorded denominators      */
static size_t rec_cap  = 0;     /* current capacity of rec array       */
static size_t rec_len  = 0;     /* number of records stored            */
static seq_t max_denom = 0;     /* maximum denominator seen so far     */
static uint64_t last_processed_prime = 2; /* prime used for the next pair */

/* Ensure the record array can hold at least 'need' entries. */
static void ensure_rec_cap(size_t need)
{
    if (rec_cap >= need) return;
    size_t new_cap = rec_cap ? rec_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(rec, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    rec = tmp;
    rec_cap = new_cap;
}

/* Initialise static data structures (once). */
static void init_structures(void)
{
    if (prime_list) return;      /* already initialised */

    /* initialise primes array with 2 */
    ensure_prime_cap(16);
    prime_list[prime_cnt++] = 2;

    /* initialise record array empty */
    ensure_rec_cap(16);
    rec_len   = 0;
    max_denom = 0;

    last_processed_prime = 2;
}

/* Generate record denominators until we have at least 'n' terms. */
static void generate_until(size_t n)
{
    if (rec_len >= n) return;
    init_structures();

    while (rec_len < n) {
        uint64_t p1 = last_processed_prime;
        uint64_t p2 = next_prime(p1);  /* next prime after p1 */

        uint64_t num = p1 - 1;
        uint64_t den = p2 - 1;
        uint64_t g   = gcd_uint64(num, den);
        uint64_t denom_val = den / g;          /* fits in 64 bits */

        seq_t denom = (seq_t)denom_val;
        if (denom > max_denom) {
            ensure_rec_cap(rec_len + 1);
            rec[rec_len++] = denom;
            max_denom = denom;
        }

        last_processed_prime = p2;   /* advance to the next prime */
    }
}

/*--- Public function ----------------------------------------------------*/

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n==0, but guard */
    generate_until((size_t)n);
    return rec[n - 1];
}
