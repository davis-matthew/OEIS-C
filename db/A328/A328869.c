/*
 * A328869  Numbers whose lengths of runs of 1's in their reversed binary
 *          expansion are weakly increasing.
 * https://oeis.org/A328869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer whose runs of 1s in the
 *            reversed binary expansion are weakly nondecreasing
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Check whether x has weakly nondecreasing run lengths of 1s in its reversed binary expansion */
static bool is_good(idx_t x)
{
    uint64_t last_len = 0;   /* length of the previous run */
    bool seen = false;       /* have we seen a run before? */
    uint64_t run = 0;        /* current run length of 1s */

    while (x) {
        if (x & 1ULL) {
            run++;          /* accumulate a 1 in the current run */
        } else {
            if (run) {
                if (seen && run < last_len)    /* violates weakly increasing rule */
                    return false;
                last_len = run;
                seen = true;
                run = 0;
            }
        }
        x >>= 1;
    }
    if (run) {                       /* handle a run that ends at the most‑significant bit */
        if (seen && run < last_len)
            return false;
    }
    return true;                      /* all runs are weakly nondecreasing */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset is 1; 0 will never be requested */

    static seq_t *cache = NULL;       /* stored sequence terms */
    static size_t cache_len = 0;      /* number of terms stored */
    static size_t cache_cap = 0;      /* current capacity of the cache array */
    static idx_t candidate = 1;       /* next number to test */

    while (cache_len < (size_t)n) {
        if (is_good(candidate)) {
            if (cache_len == cache_cap) {   /* grow the cache if necessary */
                cache_cap = cache_cap ? cache_cap * 2 : 1;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)candidate;  /* store the good number */
        }
        candidate++;                      /* advance to next integer to test */
    }
    return cache[(size_t)n - 1];          /* return the n-th term (1‑based) */
}
