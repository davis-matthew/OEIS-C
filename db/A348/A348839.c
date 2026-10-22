/*
 * A348839  Composite numbers all of whose prime factors are Mersenne primes.
 * https://oeis.org/A348839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate all composite numbers whose prime factors are Mersenne primes.
   The list is produced in increasing order using a modified Hamming‑number
   generator.  The first composite is 9 (=3^2). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* number of precomputed Mersenne primes used for the generator */
#define MER_PRIME_COUNT 9

/* the first nine Mersenne primes  (2^p-1 for p prime) */
static const seq_t mersenne_primes[MER_PRIME_COUNT] = {
    (seq_t)3ULL,                 /* 2^2 - 1 */
    (seq_t)7ULL,                 /* 2^3 - 1 */
    (seq_t)31ULL,                /* 2^5 - 1 */
    (seq_t)127ULL,               /* 2^7 - 1 */
    (seq_t)8191ULL,              /* 2^13 - 1 */
    (seq_t)131071ULL,            /* 2^17 - 1 */
    (seq_t)524287ULL,            /* 2^19 - 1 */
    (seq_t)2147483647ULL,        /* 2^31 - 1 */
    (seq_t)2305843009213693951ULL/* 2^61 - 1 */
};

/* ------------------------------------------------------------ */
/*  a(n) – n‑th composite number all of whose prime factors are
 *  Mersenne primes.  The sequence starts at n = 1.                    */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* static state – persists between calls */
    static seq_t *numbers      = NULL;   /* all generated numbers (incl. primes) */
    static size_t numbers_cap  = 0;
    static size_t numbers_len  = 0;

    static seq_t *composites   = NULL;   /* only composite numbers */
    static size_t composites_cap = 0;
    static size_t composites_len = 0;

    static size_t idxp[MER_PRIME_COUNT] = {0};   /* indices into 'numbers' */
    static bool initialized = false;

    /* initialisation on first call */
    if (!initialized) {
        numbers_cap   = 32;
        numbers       = (seq_t*)malloc(numbers_cap * sizeof(seq_t));
        if (!numbers) return 0;
        numbers[0]    = (seq_t)1;   /* sentinel for the generator */
        numbers_len   = 1;

        composites_cap = 32;
        composites     = (seq_t*)malloc(composites_cap * sizeof(seq_t));
        if (!composites) return 0;
        composites_len = 0;

        /* indices already zeroed by definition of 'idxp' */
        initialized = true;
    }

    /* generate composites until we have at least 'n' of them */
    while (composites_len < n) {
        /* compute next candidates from each Mersenne prime */
        seq_t cand[MER_PRIME_COUNT];
        for (size_t i = 0; i < MER_PRIME_COUNT; ++i) {
            cand[i] = mersenne_primes[i] * numbers[idxp[i]];
        }

        /* find minimum candidate */
        seq_t min_val = cand[0];
        size_t min_idx = 0;
        for (size_t i = 1; i < MER_PRIME_COUNT; ++i) {
            if (cand[i] < min_val) {
                min_val = cand[i];
                min_idx = i;
            }
        }

        /* append to the list of all numbers */
        if (numbers_len == numbers_cap) {
            numbers_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(numbers, numbers_cap * sizeof(seq_t));
            if (!tmp) return 0;
            numbers = tmp;
        }
        numbers[numbers_len++] = min_val;

        /* increment indices for all primes that produced this minimum */
        for (size_t i = 0; i < MER_PRIME_COUNT; ++i) {
            if (cand[i] == min_val)
                idxp[i]++;
        }

        /* check whether this minimum is prime (one of the mersenne primes) */
        bool is_prime = false;
        for (size_t i = 0; i < MER_PRIME_COUNT; ++i) {
            if (min_val == mersenne_primes[i]) {
                is_prime = true;
                break;
            }
        }

        /* if composite, add to the composites list */
        if (!is_prime) {
            if (composites_len == composites_cap) {
                composites_cap *= 2;
                seq_t *tmpc = (seq_t*)realloc(composites, composites_cap * sizeof(seq_t));
                if (!tmpc) return 0;
                composites = tmpc;
            }
            composites[composites_len++] = min_val;
        }
    }

    /* return the n‑th composite (1‑based index) */
    return composites[n - 1];
}
