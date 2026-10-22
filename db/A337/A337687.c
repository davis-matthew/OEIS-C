/*
 * A337687  a(1) = 2; for n > 1, a(n) = smallest number not occurring earlier
 *          which shares a prime factor with a(n-1) and also has a prime
 *          factor which is not a factor of a(n-1).
 * https://oeis.org/A337687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (search based):
   a(1) = 2;
   for n > 1: a(n) = smallest integer m not occurring earlier
   such that
        – m shares at least one prime factor with a(n‑1), and
        – m has at least one prime factor not dividing a(n‑1). */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper from the shared utilities – builds a prime list up to a limit. */
#include "../utils/build_primes_upto.h"

/* --------------------------------------------------------------------- */
/*  Limitations: the current implementation is meant for n ≤ 10000.
   It uses static storage for up to 200 000 terms and for a bitmap of
   numbers up to 2 000 000.  The sequence values for n≤10000 are
   comfortably inside these bounds in all supported build configurations. */
/* --------------------------------------------------------------------- */

#define CACHE_MAX (200000)
#define USED_MAX  (2000000)

/* Storage for the sequence – plain static array to avoid
   dynamic allocation of class objects (which would require placement
   new for the arbitrary‑precision build). */
static seq_t cache[CACHE_MAX];
static size_t cache_len = 0;

/* Bitmap of numbers that have already appeared.  The bitmap
   is indexed by the numeric value of the term, cast to uint64_t.
   The array is large enough for all terms up to n≈10000. */
static unsigned char used[USED_MAX];

/* Prime list used for trial division. */
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;

/* Flag to perform one‑time initialization. */
static bool initialized = false;

/* --------------------------------------------------------------------- */
/*   Checks whether candidate satisfies the recurrence conditions.      */
/* --------------------------------------------------------------------- */
static bool candidate_valid(seq_t cand, seq_t prev)
{
    bool has_common = false;
    bool has_new    = false;
    seq_t temp = cand;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        seq_t spq = (seq_t)((uint64_t)p * (uint64_t)p);
        if (spq > temp) break;

        if (temp % (seq_t)p == 0) {
            if (prev % (seq_t)p == 0)
                has_common = true;
            else
                has_new = true;
            while (temp % (seq_t)p == 0)
                temp /= (seq_t)p;
        }
    }

    if (temp > (seq_t)1) {          /* remaining prime factor */
        if (prev % temp == 0)
            has_common = true;
        else
            has_new = true;
    }

    return has_common && has_new;
}

/* --------------------------------------------------------------------- */
/*   One‑time initialization of global data.                             */
/* --------------------------------------------------------------------- */
static void initialize(void)
{
    if (initialized) return;

    /* Build a list of primes sufficient for all needed factorisations. */
    const idx_t PRIME_LIMIT = 200000;
    build_primes_upto(PRIME_LIMIT, &prime_list, &prime_cnt);

    /* Initialise the cache with the first term. */
    cache[0] = (seq_t)2;
    cache_len = 1;

    /* Mark the first term as used. */
    uint64_t val = 2;
    if (val < USED_MAX) used[val] = 1;

    initialized = true;
}

/* --------------------------------------------------------------------- */
/*   Public sequence function.                                          */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n == 0 */

    initialize();

    /* Ensure that we have computed up to the requested index. */
    while (cache_len < (size_t)n) {
        seq_t prev = cache[cache_len - 1];
        seq_t cand = (seq_t)2;

        while (1) {
            /* Cast cand to uint64_t for bitmap lookup.  Values larger
               than USED_MAX are ignored – they are certainly not
               duplicated within the inspected range. */
            uint64_t uval = (uint64_t)cand;
            if (uval < USED_MAX && used[uval]) {
                ++cand;
                continue;
            }

            if (candidate_valid(cand, prev)) {
                /* Store the new term. */
                cache[cache_len++] = cand;
                if (uval < USED_MAX) used[uval] = 1;
                break;
            }
            ++cand;
        }
    }
    return cache[n - 1];
}
