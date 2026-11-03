/*
 * A255229  Integers n such that n^2 - 1 is the difference of the squares of
 *          twin primes.
 * https://oeis.org/A255229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 33, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A255229: Integers n such that n^2-1 is the difference of the squares of twin primes.
   For odd n, let p = (n^2-5)/4.  Then n is in the sequence iff p and p+2 are both prime. */

/* Note: This implementation caches computed terms and searches for the next valid n
   by testing successive odd integers, using the helper function `is_prime_small`
   for primality checking and `append_candidate` to grow the cache. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

static seq_t *seq_cache = NULL;
static size_t seq_len   = 0;   /* number of terms currently cached    */
static size_t seq_cap   = 0;   /* capacity of seq_cache array          */
static idx_t next_candidate = 5;   /* next odd integer to be tested       */

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1, return 0 as safe default */

    /* Generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        /* Compute p = (candidate^2 - 5) / 4 */
        idx_t cand = next_candidate;
        idx_t cand_sq = cand * cand;           /* squared, fits in 64‑bit here */
        idx_t p_int  = (cand_sq - 5) / 4;      /* integer division */
        seq_t p      = (seq_t)p_int;
        seq_t p_plus2 = p + 2;

        /* Test twin‑prime condition */
        if (is_prime_seq(p) && is_prime_seq(p_plus2)) {
            append_candidate(&seq_cache, &seq_len, &seq_cap, (seq_t)cand);
        }

        /* Advance to next odd integer */
        next_candidate += 2;
    }

    return seq_cache[n - 1];
}
