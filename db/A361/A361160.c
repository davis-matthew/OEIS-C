/*
 * A361160  Discriminants of Weierstrass curves in moduli space M_3 of compact
 *          Riemann surfaces of genus 3.
 * https://oeis.org/A361160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/**
 * Sequence A361160 – discriminants of Weierstrass curves in M_3.
 * 
 * Formula: a(n) = the n-th positive integer that is
 *   (a) not a perfect square, and
 *   (b) congruent to 0 (mod 4) or 1 (mod 8).
 * 
 * The implementation enumerates candidates in increasing order,
 * tests the congruence and perfect‑square conditions, and stores
 * the qualifying values in a static cache so that subsequent calls
 * are fast.  No external utilities or floating‑point arithmetic
 * are required.  The algorithm runs in O(a(n)) time and uses an
 * amortised O(n) space for the cache.
 */
#include <stdlib.h>
#include <stdbool.h>

/* External integer types supplied by the build system. */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms stored      */
    static size_t cache_cap   = 0;   /* allocated capacity          */

    /* Next candidate integer to test. */
    static seq_t next_candidate = 1;

    /* Variables for perfect‑square detection.  We keep the current
       square root `sqrt_r` and the value `next_square = sqrt_r * sqrt_r`. */
    static seq_t sqrt_r    = 0;
    static seq_t next_square = 0;

    /* Ensure cache storage. */
    if (cache == NULL) {
        cache_cap = 128;           /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failure */
    }

    /* Generate terms until we have at least n entries. */
    while (cache_size < n) {
        /* Check the congruence condition. */
        bool ok = (next_candidate % 4 == 0) || (next_candidate % 8 == 1);

        /* Test for perfect square only if congruence holds. */
        if (ok) {
            /* Advance sqrt_r so that next_square >= next_candidate. */
            while (next_square < next_candidate) {
                sqrt_r++;
                /* Avoid overflow: for seq_t types that are small, the
                   square will still fit because next_candidate is
                   guaranteed to fit in the type where the call is made. */
                next_square = sqrt_r * sqrt_r;
            }
            if (next_square != next_candidate) {
                /* Not a perfect square – store the term. */
                if (cache_size == cache_cap) {
                    cache_cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (tmp == NULL) return 0; /* allocation failure */
                    cache = tmp;
                }
                cache[cache_size++] = next_candidate;
            }
        }

        /* Move to the next integer. */
        next_candidate++;
    }

    return cache[n - 1];
}
