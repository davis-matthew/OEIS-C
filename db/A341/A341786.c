/*
 * A341786  Norms of prime ideals in Z[(1+sqrt(-15))/2], the ring of integers
 *          of Q(sqrt(-15)).
 * https://oeis.org/A341786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 54, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A341786: Norms of prime ideals in ℤ[(1+√-15)/2].
   The set consists of
       * all primes p ≡ 1,2,3,4,5,8 (mod 15), and
       * all squares p² where p ≡ 7,11,13,14 (mod 15).
   The function builds this set up to the nth element. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/floor_sqrt.h"

/* residues for which a prime p gives a prime ideal of norm p */
static const unsigned char prime_ok[15] = {
    0, /* 0  */
    1, /* 1  */
    1, /* 2  */
    1, /* 3  */
    1, /* 4  */
    1, /* 5  */
    0, /* 6  */
    0, /* 7  */
    1, /* 8  */
    0, /* 9  */
    0, /* 10 */
    0, /* 11 */
    0, /* 12 */
    0, /* 13 */
    0  /* 14 */
};

/* residues for which a prime p gives a prime ideal of norm p² */
static const unsigned char square_ok[15] = {
    0, /* 0  */
    0, /* 1  */
    0, /* 2  */
    0, /* 3  */
    0, /* 4  */
    0, /* 5  */
    0, /* 6  */
    1, /* 7  */
    0, /* 8  */
    0, /* 9  */
    0, /* 10 */
    1, /* 11 */
    0, /* 12 */
    1, /* 13 */
    1  /* 14 */
};

static int compare_seq(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined below offset, return 0 */

    /* initial estimate for the maximum value needed */
    idx_t limit = n * 20;
    if (limit < 200) limit = 200;

    while (1) {
        /* get all primes up to the current limit */
        size_t prime_cnt = 0;
        uint64_t *primes = NULL;
        build_primes_upto(limit, &primes, &prime_cnt);

        /* maximum prime whose square can appear */
        seq_t sqrt_limit_seq = floor_sqrt((seq_t)limit);
        uint64_t sqrt_limit = (uint64_t)sqrt_limit_seq;

        /* dynamic array to hold the set of norms */
        size_t cap = prime_cnt * 2 + 10;
        seq_t *vals = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!vals) abort();

        size_t len = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            unsigned int r = (unsigned int)(p % 15);

            if (prime_ok[r]) {
                if (len == cap) {
                    cap *= 2;
                    vals = (seq_t *)realloc(vals, cap * sizeof(seq_t));
                    if (!vals) abort();
                }
                vals[len++] = (seq_t)p;
            }

            if (square_ok[r] && p <= sqrt_limit) {
                if (len == cap) {
                    cap *= 2;
                    vals = (seq_t *)realloc(vals, cap * sizeof(seq_t));
                    if (!vals) abort();
                }
                vals[len++] = (seq_t)p * (seq_t)p;
            }
        }
        free(primes);

        if (len < n) {
            free(vals);
            /* increase limit and try again */
            limit *= 2;
            continue;
        }

        /* we now have at least n values; sort and pick the nth smallest */
        qsort(vals, len, sizeof(seq_t), compare_seq);
        seq_t result = vals[n - 1];
        free(vals);
        return result;
    }
}
