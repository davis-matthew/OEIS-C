/*
 * A294717  Numbers k such that 2^((k-1)/3) == 1 (mod k) and
 *          (2*k-1)*(2^((k-1)/6)) == 1 (mod k).
 * https://oeis.org/A294717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 552, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that 2^((k-1)/3) == 1 (mod k) and (2k-1)*2^((k-1)/6) == 1 (mod k). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

#define CACHE_CAP 32768   /* sufficient for n up to about 10 000 */

/* static cache holding the terms of the sequence */
static seq_t cache[CACHE_CAP];
static size_t cache_len = 0;      /* number of terms currently stored */
static uint64_t next_k = 1;       /* next k to be tested (k ≡ 1 (mod 6)) */
static bool initialized = false; /* whether the cache has been seeded */

/* returns the n-th term (1‑indexed) of A294717 */
seq_t a(idx_t n)
{
    /* initialize cache with the first term */
    if (!initialized) {
        cache[0] = (seq_t)1;          /* a(1) = 1 */
        cache_len = 1;
        /* next_k is already 1; it will be incremented by 6 in the loop below */
        initialized = true;
    }

    /* grow the cache until it contains at least n terms */
    while (cache_len < n) {
        do {
            /* move to the next candidate (only k ≡ 1 (mod 6) can work) */
            next_k += 6ULL;
        } while (next_k > 1 && (next_k % 6ULL) != 1ULL);

        /* first congruence: 2^((k-1)/3) ≡ 1 (mod k) */
        uint64_t exp1 = (next_k - 1ULL) / 3ULL;
        seq_t  term1 = mod_pow((seq_t)2, exp1, (seq_t)next_k);
        if (term1 != (seq_t)1)
            continue;   /* not a member, try next k */

        /* second congruence reduces to 2^((k-1)/6) ≡ -1 (mod k) */
        uint64_t exp2 = (next_k - 1ULL) / 6ULL;
        seq_t  term2 = mod_pow((seq_t)2, exp2, (seq_t)next_k);
        if (term2 != ((seq_t)next_k - (seq_t)1))
            continue;   /* not a member */

        /* both conditions satisfied: store the term */
        if (cache_len == CACHE_CAP) {
            /* cache is full – this should not happen for requested n */
            /* silently stop adding more terms */
        } else {
            cache[cache_len++] = (seq_t)next_k;
        }
    }

    return cache[n - 1];
}
