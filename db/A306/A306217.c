/*
 * A306217  Sphenic numbers with a prime nearest neighbor.
 * https://oeis.org/A306217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 2059, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9747 (2026-11-03)
 * Helpers:   build_spf, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A306217: 2·p·q with distinct odd primes p<q such that either 2·p·q − 1 or 2·p·q + 1 is prime.
   The implementation constructs the sequence by progressively searching integers, starting
   with a small sieve of smallest prime factors (SPF) and checking primality of the neighbouring
   integers.  Cache and SPF arrays grow as needed to support arbitrary indices. */

/* ------------------------------------------------------------------ */
/*  Includes and forward declarations                                 */
/* ------------------------------------------------------------------ */

#include <stdlib.h>
#include <stdbool.h>

/* Utilities supplied by the build system */
#include <stdint.h>
#include "../utils/build_spf.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Static mutable state – shared between invocations of a()          */
/* ------------------------------------------------------------------ */

static seq_t   *cache      = NULL;   /* stored sequence terms               */
static size_t  cache_cap   = 0;      /* allocated capacity of cache          */
static size_t  cache_len   = 0;      /* current number of stored terms       */

static idx_t  *spf        = NULL;   /* smallest prime factor table          */
static idx_t   spf_limit  = 0;      /* highest number indexed by spf        */

static idx_t  last_checked = 0;     /* largest integer that has already been examined */

/* ------------------------------------------------------------------ */
/*  Ensure SPF table covers at least the argument.                    */
/* ------------------------------------------------------------------ */
static void
ensure_spf_cover(idx_t up_to)
{
    if (up_to <= spf_limit)
        return;

    /* Grow the limit geometrically until large enough. */
    idx_t new_limit = spf_limit ? spf_limit : 64;
    while (new_limit < up_to)
        new_limit <<= 1;

    idx_t *new_spf = NULL;
    build_spf(new_limit, &new_spf);

    free(spf);
    spf       = new_spf;
    spf_limit = new_limit;
}

/* ------------------------------------------------------------------ */
/*  Test whether n is of the form 2·p·q with distinct odd primes.    */
/* ------------------------------------------------------------------ */
static bool
is_two_pq_sphenic(idx_t n)
{
    /* Must be even and at least 2·3·5 = 30. */
    if (n < 30 || (n & 1u) != 0)
        return false;

    idx_t m = n >> 1;  /* factor out the 2 */

    /* SPF must cover m. */
    if (spf == NULL || m > spf_limit)
        return false;

    /* First prime factor p. */
    idx_t p = spf[m];
    if (p == 0 || p > m || p % 2 == 0)
        return false;

    m /= p;                   /* remove p */

    /* Remaining part must be a different odd prime. */
    if (m == 1 || m == p)
        return false;

    if (m <= spf_limit) {
        if (spf[m] != m)   /* m is not prime */
            return false;
    } else {
        if (!is_prime_big((seq_t)m))
            return false;
    }

    return true;              /* n = 2·p·q with distinct odd primes */
}

/* ------------------------------------------------------------------ */
/*  Extend the cache until it contains at least 'target' terms.      */
/* ------------------------------------------------------------------ */
static void
extend_cache_until(idx_t target)
{
    /* Guard against overflow when seq_t is small. */
    const bool seq_t_is_small = sizeof(seq_t) <= sizeof(uint64_t);

    while (cache_len < target) {
        /* Scan integers sequentially. */
        ++last_checked;

        /* Ensure we can factor up to this number. */
        ensure_spf_cover(last_checked);

        /* Stop if value would not fit in seq_t. */
        if (seq_t_is_small) {
            seq_t max_seq = ~(seq_t)0;
            if ((idx_t)last_checked > (idx_t)max_seq)
                break;
        }

        if (is_two_pq_sphenic(last_checked)) {
            /* Either neighbour must be prime. */
            bool prime_neighbour =
                is_prime_big((seq_t)last_checked - 1) ||
                is_prime_big((seq_t)last_checked + 1);

            if (prime_neighbour) {
                /* Allocate space if necessary. */
                if (cache_len >= cache_cap) {
                    cache_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                    cache = mem_resize(seq_t, cache, cache_cap);
                }
                cache[cache_len++] = (seq_t)last_checked;
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Public entry point.                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* One‑time initialization. */
    static bool initialized = false;
    if (!initialized) {
        /* Start with a reasonable sieve size.  It will be enlarged automatically. */
        const idx_t INIT_LIMIT = 4000000;   /* enough to find many initial terms */
        build_spf(INIT_LIMIT, &spf);
        spf_limit = INIT_LIMIT;

        cache_cap   = 64;
        cache_len   = 0;
        cache       = mem_new(seq_t, cache_cap);
        last_checked = 2;   /* scanning will start from 3 */

        initialized = true;
    }

    /* The offset of the sequence is 1; values for n < 1 are undefined. */
    if (n == 0)
        return 0;

    extend_cache_until(n);
    return cache[(size_t)n - 1];
}
