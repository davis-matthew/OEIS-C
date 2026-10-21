/*
 * A242702  Semiprimes n such that n^2+n+41 is also semiprime.
 * https://oeis.org/A242702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 24, u32 n <= 8194, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, count_prime_factors, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* semiprimes n such that n^2+n+41 is also semiprime */
/* This implementation searches integers sequentially and caches the results. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/count_prime_factors.h"
#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"

/* ------------------------------------------------------------------ */
/* Dynamic array helpers                                              */
/* ------------------------------------------------------------------ */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed > *cap) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        while (new_cap < needed) new_cap <<= 1;
        *ptr = realloc(*ptr, new_cap * elem_size);
        *cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/* Int‑prime table for fast trial division                           */
/* ------------------------------------------------------------------ */
static uint64_t *primes = NULL;   /* array of primes          */
static size_t prime_count = 0;    /* number of primes in array*/

static void init_prime_table(void)
{
    if (primes != NULL) return;
    /* enough to factor candidates up to about 10‑12 million (sqrt ~3 600) */
    /* we generate primes up to 2 000 000 – > 1.4 × 10^5 primes */
    build_primes_upto(2000000, &primes, &prime_count);
}

/* ------------------------------------------------------------------ */
/* Helper: test whether a 64‑bit integer is a semiprime              */
/* ------------------------------------------------------------------ */
static bool is_semiprime_uint64(uint64_t x)
{
    if (x < 4) return false;          /* smallest semiprime is 4 = 2*2 */
    uint64_t cnt  = 0;
    uint64_t tmp  = x;
    size_t i;

    init_prime_table();

    for (i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if (p * p > tmp) break;
        while (tmp % p == 0) {
            ++cnt;
            tmp /= p;
            if (cnt > 2) return false;
        }
    }
    if (tmp > 1) ++cnt;   /* remaining prime factor */

    return cnt == 2;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 4;   /* start scanning after 1, 2, 3 */

    /* Ensure we have the required element */
    while (cache_len < (size_t)n) {
        seq_t cand  = (seq_t)next_candidate;

        /* ----- check if cand is semiprime ----- */
        bool cand_semiprime;
        if (cand <= (seq_t)UINT64_MAX) {
            cand_semiprime = is_semiprime_uint64((uint64_t)cand);
        } else {
            cand_semiprime = (count_prime_factors(cand) == 2);
        }

        if (cand_semiprime) {
            /* m = n^2 + n + 41 */
            seq_t m = cand * cand + cand + (seq_t)41;

            /* ----- check if m is semiprime ----- */
            bool m_semiprime;
            if (m <= (seq_t)UINT64_MAX) {
                m_semiprime = is_semiprime_uint64((uint64_t)m);
            } else {
                m_semiprime = (count_prime_factors(m) == 2);
            }

            if (m_semiprime) {
                ensure_capacity(cache_len + 1, &cache_cap, (void **)&cache, sizeof(seq_t));
                cache[cache_len++] = cand;
            }
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
