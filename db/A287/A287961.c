/*
 * A287961  Numbers that are the sum of two palindromic primes (A002385).
 * https://oeis.org/A287961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   build_primes_upto, is_palindrome, push_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Find all palindromic primes up to LIMIT, generate all sums of two
   of them, collect the distinct sums in increasing order, and return the nth
   term (offset 1). */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/is_palindrome.h"
#include "../utils/push_to_seq.h"

static const idx_t LIMIT = 2000000ULL;          /* primes up to 2,000,000 */
static seq_t *sums    = NULL;                   /* sorted distinct sums   */
static size_t sums_cnt = 0;                     /* number of sums         */
static bool initialized = false;               /* initialization flag   */

/* Initialize the sequence data on first call. */
static void init_seq(void)
{
    if (initialized) return;
    initialized = true;

    /* Build all primes up to LIMIT. */
    uint64_t *all_primes = NULL;
    size_t primes_cnt = 0;
    build_primes_upto(LIMIT, &all_primes, &primes_cnt);

    /* Extract palindromic primes. */
    seq_t *palprimes = NULL;
    size_t pal_cnt   = 0, pal_cap = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        seq_t p = (seq_t)all_primes[i];
        if (is_palindrome(p, 10)) {
            push_to_seq(&palprimes, &pal_cnt, &pal_cap, p);
        }
    }
    free(all_primes); /* not needed further */

    /* Mark sums of two palindromic primes. */
    size_t max_sum = (size_t)(LIMIT * 2ULL);
    bool *seen = (bool *)calloc(max_sum + 1, sizeof(bool));
    for (size_t i = 0; i < pal_cnt; ++i) {
        for (size_t j = i; j < pal_cnt; ++j) {
            seq_t s = palprimes[i] + palprimes[j];
            seen[(size_t)s] = true;
        }
    }

    /* Collect all distinct sums into sorted array. */
    sums_cnt = 0;
    sums = NULL;
    size_t sums_cap = 0;
    for (size_t s = 4; s <= max_sum; ++s) { /* sums start at 4 */
        if (seen[s]) push_to_seq(&sums, &sums_cnt, &sums_cap, (seq_t)s);
    }

    free(seen);
    free(palprimes);
}

/* External linkage function required by the build system. */
seq_t a(idx_t n)
{
    init_seq();
    if (n == 0 || n > sums_cnt) return 0; /* safety, should not happen */
    return sums[n - 1];
}
