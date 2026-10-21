/*
 * A267572  Number of steps J. H. Conway's Fractran program needs to calculate
 *          the n-th prime when starting from 2^n.
 * https://oeis.org/A267572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps of Conway's Fractran program {17/91,
 *            78/85, 19/51, 23/38, 29/33, 77/29, 95/23, 77/19, 1/17, 11/13,
 *            13/11, 15/14, 15/2, 55/1} starting from 2^n and stopping when
 *            2^prime(n) is reached
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation precomputes the fractions' prime exponents once,
    uses a small fixed sieve to obtain the first few primes, and
    memoises computed values in a static cache.  The algorithm is
    O(a(n)) per request, with a very small constant factor. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Prime factorisation of the 14 fractions – only primes up to 29 are needed */

static const int small_primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

static const int fr_num[14] = {
    17, 78, 19, 23, 29, 77, 95, 77, 1, 11, 13, 15, 15, 55
};

static const int fr_den[14] = {
    91, 85, 51, 38, 33, 29, 23, 19, 17, 13, 11, 14, 2, 1
};

static int num_exp[14][10];
static int den_exp[14][10];
static bool fracs_done = false;

/* Compute prime exponents of all numerators and denominators once. */
static void init_fracs(void) {
    if (fracs_done) return;
    for (int f = 0; f < 14; ++f) {
        int v = fr_num[f];
        for (int k = 0; k < 10; ++k) {
            int p = small_primes[k];
            while (v % p == 0) {
                ++num_exp[f][k];
                v /= p;
            }
        }
        v = fr_den[f];
        for (int k = 0; k < 10; ++k) {
            int p = small_primes[k];
            while (v % p == 0) {
                ++den_exp[f][k];
                v /= p;
            }
        }
    }
    fracs_done = true;
}

/* --------------------------------------------------------------------- */
/* Simple sieve to obtain the first ~20 000 primes (enough for n ≤ 10 000) */

#define PRIME_SIEVE_LIMIT 200000   /* > 104 729 (the 10 000th prime) */

static uint32_t primes[20000];  /* safer to store only needed amount */
static size_t prime_cnt = 0;
static bool primes_done = false;

static void init_primes(void) {
    if (primes_done) return;
    bool *is_composite = (bool *)calloc(PRIME_SIEVE_LIMIT + 1, sizeof(bool));
    if (!is_composite) return; /* unlikely */
    size_t idx = 0;
    for (uint32_t p = 2; p <= PRIME_SIEVE_LIMIT; ++p) {
        if (!is_composite[p]) {
            if (idx < 20000) primes[idx++] = p;
            if (p <= PRIME_SIEVE_LIMIT / p) {
                for (uint32_t m = p * p; m <= PRIME_SIEVE_LIMIT; m += p)
                    is_composite[m] = true;
            }
        }
    }
    prime_cnt = idx;
    primes_done = true;
    free(is_composite);
}

/* Helper: get the n-th prime (1‑based).  Failed lookups return 0. */
static uint32_t get_prime(const idx_t n) {
    if (!primes_done) init_primes();
    if (n == 0 || n > prime_cnt) return 0;
    return primes[n - 1];
}

/* --------------------------------------------------------------------- */
/* Cache for computed sequence values */

static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Ensure cache has room for indices up to n.  New entries are zeroed. */
static void ensure_cache(const idx_t n) {
    if ((size_t)n < cache_size) return;
    size_t new_size = (size_t)n + 1;          /* index 0 unused */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return;                         /* allocation failure – unlikely */
    /* initialize new slots to 0 */
    if (cache_size < new_size) {
        for (size_t i = cache_size; i < new_size; ++i) tmp[i] = 0;
    }
    cache = tmp;
    cache_size = new_size;
}

/* --------------------------------------------------------------------- */
/* Main function required by the problem statement */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;         /* for safety: offset is 1 */

    if (cache == NULL) cache = NULL;      /* just to silence unused warning */

    /* initialise helpers */
    init_fracs();
    init_primes();

    ensure_cache(n);

    if (cache[n] != (seq_t)0) return cache[n];

    /* Target prime exponent on the base 2 side */
    uint32_t target_prime = get_prime(n);
    if (target_prime == 0) {
        cache[n] = (seq_t)0;
        return (seq_t)0;                /* invalid n */
    }

    /* Exponent vector for current value; 2..29 are tracked */
    int exp[10] = {0};
    exp[0] = (int)n;                     /* exponent of 2 starts at n */

    seq_t steps = (seq_t)0;

    for (;;) {
        /* check if we have reached 2^prime(n) */
        if (exp[0] == (int)target_prime) {
            bool all_zero = true;
            for (int k = 1; k < 10 && all_zero; ++k)
                if (exp[k] != 0) all_zero = false;
            if (all_zero) break;
        }

        /* find first applicable fraction */
        for (int f = 0; f < 14; ++f) {
            bool ok = true;
            for (int k = 0; k < 10; ++k) {
                if (exp[k] + num_exp[f][k] < den_exp[f][k]) {
                    ok = false;
                    break;
                }
            }
            if (!ok) continue;

            /* apply fraction: adjust exponents */
            for (int k = 0; k < 10; ++k)
                exp[k] += num_exp[f][k] - den_exp[f][k];

            ++steps;
            break;  /* proceed to next iteration */
        }
    }

    cache[n] = steps;
    return steps;
}
