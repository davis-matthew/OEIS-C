/*
 * A301296  Smallest distance from n to a prime power (as defined in A246547).
 * https://oeis.org/A301296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the smallest distance from n to a prime power.
   The function builds a table of all prime powers up to a growing
   limit and then binary‑searches it for the nearest value. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- helper for sorting the power list ---------------------------------- */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

/* --- static storage for the list of prime powers ------------------------ */
static uint64_t *prime_powers = NULL; /* the list, sorted ascending   */
static size_t pp_cnt       = 0;        /* number of entries stored     */
static size_t pp_cap       = 0;        /* current allocated capacity   */
static uint64_t pp_limit   = 0;        /* maximum value stored so far */

/* ----- generate prime powers up to `limit` -------------------------------- */
static void ensure_prime_powers(uint64_t limit)
{
    if (limit <= pp_limit)
        return;                       /* already have enough */

/* 1. Sieve primes up to sqrt(limit) -------------------------------------- */
    uint64_t sqrt_limit = 0;
    while ((sqrt_limit + 1) * (sqrt_limit + 1) <= limit)
        sqrt_limit++;

    size_t sieve_size = (size_t)sqrt_limit + 1;
    char *is_prime = (char *)malloc(sieve_size);
    if (!is_prime)
        return;                  /* out of memory – unlikely for reasonable limits */

    for (size_t i = 0; i < sieve_size; ++i)
        is_prime[i] = 1;
    if (sieve_size > 0) is_prime[0] = 0;
    if (sieve_size > 1) is_prime[1] = 0;

    for (size_t i = 2; i * i < sieve_size; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j < sieve_size; j += i)
                is_prime[j] = 0;
        }
    }

/* 2. Grow the power array if needed ------------------------------------- */
    if (pp_cap == 0) {
        pp_cap = 16;
        prime_powers = (uint64_t *)malloc(pp_cap * sizeof(uint64_t));
    }

/* 3. Generate powers for each prime, adding only new values -------------- */
    for (size_t i = 2; i < sieve_size; ++i) {
        if (!is_prime[i])
            continue;
        uint64_t p = (uint64_t)i;

        /* Start at p^2 */
        uint64_t val = p * p;
        while (val <= limit) {
            if (val > pp_limit) {            /* new value, not yet stored */
                if (pp_cnt == pp_cap) {       /* grow array if full */
                    pp_cap = pp_cap * 2 + 16;
                    prime_powers = (uint64_t *)realloc(prime_powers,
                                                        pp_cap * sizeof(uint64_t));
                }
                prime_powers[pp_cnt++] = val;
            }
            /* next power, guard against overflow */
            if (val > limit / p)
                break;
            val *= p;
        }
    }

    free(is_prime);

/* 4. Sort the list and remove any accidental duplicates ----------------- */
    if (pp_cnt > 1) {
        qsort(prime_powers, pp_cnt, sizeof(uint64_t), cmp_uint64);
        size_t uniq = 0;
        for (size_t i = 0; i < pp_cnt; ++i) {
            if (uniq == 0 || prime_powers[i] != prime_powers[uniq - 1])
                prime_powers[uniq++] = prime_powers[i];
        }
        pp_cnt = uniq;
    }

    pp_limit = limit;
}

/* ----- main sequence function -------------------------------------------- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        /* Initial precomputation – enough for all test values up to ~10⁴. */
        ensure_prime_powers(2000000ull);
        initialized = true;
    }

    uint64_t un = (uint64_t)n;
    if (un > pp_limit) {
        /* Ensure that we have at least one power above n. */
        ensure_prime_powers(un + 1000ull);
    }

    /* Binary search for the first power >= n */
    size_t lo = 0, hi = pp_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_powers[mid] < un)
            lo = mid + 1;
        else
            hi = mid;
    }

    uint64_t dist;
    if (lo == 0) {                         /* all powers above n */
        dist = prime_powers[0] - un;
    } else if (lo == pp_cnt) {             /* all powers below n */
        dist = un - prime_powers[pp_cnt - 1];
    } else {                               /* neighbours exist */
        uint64_t d1 = un - prime_powers[lo - 1];
        uint64_t d2 = prime_powers[lo] - un;
        dist = (d1 < d2) ? d1 : d2;
    }

    return (seq_t)dist;                    /* cast back to seq_t */
}
