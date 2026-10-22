/*
 * A306676  Number of distinct acute triangles with prime sides and largest
 *          side = prime(n).
 * https://oeis.org/A306676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A306676: Number of distinct acute triangles with prime sides
   and largest side = prime(n).  For each prime a ≤ p with p = prime(n)
   count the primes b with
         a ≤ b ≤ p,
         a + b > p           // triangle inequality
         a^2 + b^2 > p^2     // acute condition
   Sum over all such a.  The algorithm generates primes up to p on‑demand
   using a simple sieve and then scans each a once.  Complexity ≈ O(p/ln p),
   meeting the 2‑second limit for n ≤ 10000. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----------  static data for on‑demand prime generation  ---------- */
static uint64_t *primes = NULL;        /* array of primes */
static size_t primes_count = 0;        /* number of primes stored */
static size_t sieve_limit = 0;         /* highest integer that has been sieved */

/* ----------  integer square root for uint64_t ------------------ */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = ((uint64_t)1) << 32;   /* > sqrt(UINT64_MAX) */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* ----------  lower bound search in primes array ------------- */
static size_t lower_bound_prime(uint64_t val)
{
    size_t lo = 0, hi = primes_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* ----------  ensure that at least 'need' primes are available  --- */
static void ensure_primes_count(idx_t need)
{
    if ((size_t)need <= primes_count)
        return;

    /* Use a heuristic upper bound for the nth prime when needed.  */
    size_t limit = sieve_limit ? sieve_limit : 100;
    while (true) {
        uint8_t *sieve = (uint8_t *)calloc(limit + 1, 1);   /* 0 = prime, 1 = composite */
        if (!sieve) return;                                 /* allocation failed – give up */

        for (size_t i = 2; i * i <= limit; ++i) {
            if (!sieve[i]) {
                for (size_t j = i * i; j <= limit; j += i)
                    sieve[j] = 1;
            }
        }

        /* Count primes up to 'limit' */
        size_t new_count = 0;
        for (size_t i = 2; i <= limit; ++i)
            if (!sieve[i])
                ++new_count;

        if (new_count >= need) {        /* enough primes found */
            uint64_t *new_primes = (uint64_t *)malloc(new_count * sizeof(uint64_t));
            if (!new_primes) {
                free(sieve);
                return;   /* allocation failed – give up */
            }
            size_t idx = 0;
            for (size_t i = 2; i <= limit; ++i)
                if (!sieve[i])
                    new_primes[idx++] = (uint64_t)i;

            free(primes);
            primes = new_primes;
            primes_count = new_count;
            sieve_limit = limit;
            free(sieve);
            break;
        }

        free(sieve);
        /* increase limit geometrically */
        limit <<= 1;
        if (limit == 0)          /* overflow guard */
            limit = SIZE_MAX;
    }
}

/* -------------------  sequence function  --------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* offset starts at 1 */

    ensure_primes_count(n);
    uint64_t p = primes[(size_t)n - 1];    /* prime(n) */
    size_t idxP = (size_t)n - 1;           /* index of prime(n) */

    seq_t total = 0;

    for (size_t i = 0; i <= idxP; ++i) {
        uint64_t a_prime = primes[i];

        /* Compute minimal possible b from triangle inequality */
        uint64_t b_min = a_prime;
        if (p > a_prime) {
            uint64_t t = p - a_prime + 1;
            if (t > b_min) b_min = t;
        }

        /* Add acute condition: a^2 + b^2 > p^2  ->  b > sqrt(p^2 - a^2) */
        __uint128_t diff128 = (__uint128_t)p * p - (__uint128_t)a_prime * a_prime;
        uint64_t diff = (uint64_t)diff128;
        uint64_t s = isqrt_u64(diff);
        uint64_t t = s + 1;
        if (t > b_min) b_min = t;

        if (b_min > p) continue;          /* no admissible b for this a */

        size_t lindex = lower_bound_prime(b_min);
        if (lindex > idxP) continue;      /* all primes > p */

        size_t count = idxP - lindex + 1;
        total += (seq_t)count;
    }

    return total;
}
