/*
 * A351597  Primes p such that the 6 consecutive primes starting with p are
 *          congruent to 11, 13, 17, 19, 23, 29 (modulo 30) in this order.
 * https://oeis.org/A351597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   grow_seq, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A351597: Primes p such that the 6 consecutive primes starting with
    p are congruent to 11,13,17,19,23,29 (mod 30) in this order.
    This routine searches for primes one by one, keeping a sliding window
    of the last six primes; whenever the window matches the required residues
    the first prime of that window is cached as the next term.
    The sequence is memoised, so calls with arbitrary n are fast. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/isPrimeGeneric.h"
#include "../utils/grow_seq.h"

/* cache of found terms */
static seq_t *cache = NULL;     /* array of found primes */
static size_t cache_cap = 0;    /* allocated capacity of cache */
static size_t cache_len = 0;    /* number of entries actually stored */

/* sliding window of the last six primes */
static seq_t win[6] = {0};
static size_t win_len = 0;

/* the prime most recently used when searching */
static seq_t last_candidate = 0;

/* returns the first prime strictly greater than start */
static seq_t next_prime(seq_t start)
{
    seq_t n = start + (seq_t)1;
    if (n <= (seq_t)2) return (seq_t)2;
    if ((n & (seq_t)1) == 0) n++;           /* make odd */
    for (;;) {
        if (isPrimeGeneric(n)) return n;
        n += (seq_t)2;                      /* test only odds */
    }
}

/* grow the cache array if needed and append a value */
static void append_cache(seq_t val)
{
    /* ensure capacity for one more element */
    grow_seq(&cache, &cache_cap, cache_len + 1);
    cache[cache_len++] = val;
}

/* required function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* behaviour below offset is unspecified */

    /* generate terms until we have at least n cached */
    while (cache_len < (size_t)n) {
        /* get the next prime */
        seq_t prime = next_prime(last_candidate);
        last_candidate = prime;

        /* update sliding window */
        if (win_len < 6) {
            win[win_len++] = prime;
        } else {
            /* shift left */
            for (size_t i = 0; i < 5; ++i) win[i] = win[i + 1];
            win[5] = prime;
        }

        /* check pattern when window is full */
        if (win_len == 6) {
            static const seq_t residues[6] = {11, 13, 17, 19, 23, 29};
            bool match = true;
            for (size_t i = 0; i < 6; ++i) {
                if ((win[i] % (seq_t)30) != residues[i]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                /* first prime of the matching block */
                append_cache(win[0]);
            }
        }
    }

    return cache[n - 1];
}
