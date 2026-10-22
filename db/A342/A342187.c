/*
 * A342187  Numbers k such that both k and k+1 are not exponentially odd
 *          numbers.
 * https://oeis.org/A342187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k (k ≥ 1) for which * both k and k+1 are
 *            NOT exponentially odd * (i.e., some prime exponent in each
 *            factorisation is even). * The function generates terms by a
 *            simple search, using a sieve of * smallest prime factors up to a
 *            fixed limit for fast factorisation
 * Range:     u8 n <= 14, u16 n <= 4125, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Smallest‑prime‑factor table up to LIMIT (for fast factorisation)  */
/* ------------------------------------------------------------------ */
#define SPF_LIMIT 1048576ul /* 2^20, comfortably covers n up to ~200 000 */

static uint32_t *spf = NULL;

/* Build SPF table once */
static void init_spf(void) {
    if (spf != NULL) {
        return;
    }
    spf = (uint32_t*)malloc((SPF_LIMIT + 1) * sizeof(uint32_t));
    if (!spf) abort();
    for (size_t i = 0; i <= SPF_LIMIT; i++)
        spf[i] = (uint32_t)i;
    for (size_t i = 2; i * i <= SPF_LIMIT; i++) {
        if (spf[i] == (uint32_t)i) {
            for (size_t j = i * i; j <= SPF_LIMIT; j += i) {
                if (spf[j] == (uint32_t)j)
                    spf[j] = (uint32_t)i;
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Check whether a 64‑bit integer is exponentially odd               */
/* ------------------------------------------------------------------ */
static bool is_exponentially_odd(uint64_t n) {
    if (n <= 1)
        return true;            /* 1 has no prime factors → vacuously true */

    while (n > 1) {
        uint32_t p, e = 0;
        if (n <= SPF_LIMIT) {
            p = spf[n];
            while (n % p == 0) {
                n /= p;
                e++;
            }
        } else {
            /* Trial division for the very rare large values */
            if (n % 2 == 0) {
                p = 2;
                while (n % 2 == 0) {
                    n /= 2;
                    e++;
                }
                if ((e & 1) == 0)
                    return false;
                e = 0; /* reset for next prime */
            }
            for (p = 3; (uint64_t)p * p <= n; p += 2) {
                if (n % p == 0) {
                    while (n % p == 0) {
                        n /= p;
                        e++;
                    }
                    if ((e & 1) == 0)
                        return false;
                    e = 0;
                }
            }
            if (n > 1) {            /* remaining prime factor has exponent 1 */
                e = 1;
                if ((e & 1) == 0)    /* will never happen */
                    return false;
            }
            return true;
        }

        if ((e & 1) == 0)
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Cache for already computed sequence terms (as uint64_t values)      */
/* ------------------------------------------------------------------ */
static uint64_t *cache = NULL;
static size_t cache_count = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 1;

/* Grow the cache if needed */
static void ensure_cache_cap(size_t needed) {
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < needed)
        new_cap *= 2;
    uint64_t *new_cache = (uint64_t*)realloc(cache, new_cap * sizeof(uint64_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Main function: returns the nth term of A342187 (n ≥ 1).             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0)          /* undefined for 0; safe placeholder */
        return (seq_t)0;

    init_spf();          /* ensure SPF table is ready */

    ensure_cache_cap((size_t)n);

    while (cache_count < (size_t)n) {
        uint64_t k = next_candidate;
        bool k_expodd   = is_exponentially_odd(k);
        bool kp1_expodd = is_exponentially_odd(k + 1);
        if (!k_expodd && !kp1_expodd) {
            cache[cache_count++] = k;
        }
        next_candidate++;
    }

    return (seq_t)cache[(size_t)n - 1];
}
