/*
 * A384568  Consecutive states of the linear congruential pseudo-random number
 *          generator for the Hewlett Packard HP-11C when started at 1.
 * https://oeis.org/A384568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A384568 (HP‑11C LCG)  
    a(1) = 1
    a(n) = (1574352261 * a(n‑1) + 1017980433) mod 10^10   for n > 1   */
#include <stdlib.h>
#include <stdint.h>

/* Modulus 10^10 fits in uint64_t */
static const uint64_t MOD = 10000000000ULL;

seq_t a(idx_t n)
{
    /* n is 1‑based.  */
    if (n == 0)
        return (seq_t)0;          /* undefined for this sequence */

    /* Convert to a size_t for array indexing (valid because n ≥ 1).  */
    size_t idx = (size_t)n;

    /* Static cache for memoisation.  */
    static seq_t *cache = NULL;
    static size_t used = 0;      /* number of values stored */
    static size_t cap  = 0;      /* allocated capacity */

    /* Grow the cache if necessary.  */
    if (idx > used) {
        /* Allocate / grow the buffer.  */
        if (idx > cap) {
            size_t new_cap = (cap == 0) ? 16 : cap * 2;
            if (new_cap < idx)
                new_cap = idx;
            seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!tmp)
                exit(1);          /* simple error handling */
            if (cache) {
                for (size_t i = 0; i < used; ++i)
                    tmp[i] = cache[i];
                free(cache);
            }
            cache = tmp;
            cap   = new_cap;
        }

        /* Initialise and/ or append values up to idx.  */
        for (size_t i = used; i < idx; ++i) {
            if (i == 0) {              /* a(1) = 1 */
                cache[i] = (seq_t)1;
            } else {
                seq_t prev = cache[i - 1];
                /* Perform multiplication and addition in 128‑bit to avoid
                   overflow for all supported seq_t sizes. */
                unsigned __int128 x =
                    (unsigned __int128)prev * 1574352261ULL + 1017980433ULL;
                x %= MOD;
                cache[i] = (seq_t)x;
            }
        }
        used = idx;
    }

    return cache[n - 1];
}
