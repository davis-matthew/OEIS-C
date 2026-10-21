/*
 * A258095  Numbers m, such that the smallest prime factor of 1+78557*2^m
 *          doesn't belong to the covering set {3, 5, 7, 13, 19, 37, 73}.
 * https://oeis.org/A258095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 187, u32 n <= 250, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of sequence A258095.
   We search for exponents m such that the smallest prime factor of
   1 + 78557·2^m is not in the covering set {3,5,7,13,19,37,73}.  */

#include <stdlib.h>
#include <stdbool.h>

enum { PRIME_COUNT = 20 };

/* Primes up to 73 (excluding 2, which never divides 1 + 78557·2^m). */
static const int primes[PRIME_COUNT] = {
    3, 5, 7, 11, 13, 17, 19, 23, 29, 31,
    37, 41, 43, 47, 53, 59, 61, 67, 71, 73
};

/* Whether each prime is in the covering set. */
static const bool cover[PRIME_COUNT] = {
    true,   /* 3  */
    true,   /* 5  */
    true,   /* 7  */
    false,  /* 11 */
    true,   /* 13 */
    false,  /* 17 */
    true,   /* 19 */
    false,  /* 23 */
    false,  /* 29 */
    false,  /* 31 */
    true,   /* 37 */
    false,  /* 41 */
    false,  /* 43 */
    false,  /* 47 */
    false,  /* 53 */
    false,  /* 59 */
    false,  /* 61 */
    false,  /* 67 */
    false,  /* 71 */
    true    /* 73 */
};

static seq_t *cache = NULL;      /* cache of found m's */
static size_t cache_len = 0;      /* number of cached values */
static size_t cache_cap = 0;      /* allocated capacity */
static idx_t next_m = 1;          /* next exponent to test */

static unsigned int pow2_mod[PRIME_COUNT];   /* 2^m mod primes[i] */
static unsigned int mult_mod[PRIME_COUNT];   /* 78557 mod primes[i] */
static bool initialized = false;

/* One‑time initialization */
static void init_static(void) {
    if (initialized) return;
    initialized = true;

    /* Allocate cache */
    cache_cap = 64;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) return;          /* allocation failure – subsequent uses will fail */

    /* Pre‑compute 78557 mod p for each prime */
    for (int i = 0; i < PRIME_COUNT; ++i)
        mult_mod[i] = (unsigned int)78557 % primes[i];

    /* Initialise pow2_mod for m = 1 (2^1 mod p). */
    for (int i = 0; i < PRIME_COUNT; ++i)
        pow2_mod[i] = 2 % primes[i];
}

/* Grow cache if needed */
static void ensure_cache(void) {
    if (cache_len < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) return;  /* allocation failure – subsequent uses will fail */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    init_static();
    if (!cache) return (seq_t)0;   /* allocation failed – return 0 */

    while (cache_len < n) {        /* generate terms until we have n of them */
        bool accept = true;         /* assume this m qualifies until proven otherwise */

        for (int i = 0; i < PRIME_COUNT; ++i) {
            /* Compute N(m) mod primes[i] = (1 + 78557·2^m) mod p */
            unsigned int product = mult_mod[i] * pow2_mod[i];
            unsigned int val = (1 + product) % primes[i];
            if (val == 0) {           /* prime divides N(m) – it's the smallest one */
                if (cover[i]) {
                    accept = false;   /* smallest prime factor is in covering set */
                } else {
                    accept = true;    /* smallest prime factor is outside covering set */
                }
                break;               /* stop at the smallest divisor */
            }
        }

        if (accept) {                /* m meets the criterion */
            ensure_cache();
            cache[cache_len++] = (seq_t)next_m;
        }

        /* Prepare for next m: increment m and update 2^m mod p for all primes. */
        ++next_m;
        for (int i = 0; i < PRIME_COUNT; ++i) {
            pow2_mod[i] = (pow2_mod[i] * 2u) % primes[i];
        }
    }

    return cache[n - 1];
}
