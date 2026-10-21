/*
 * A296889  Numbers whose base-12 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          > #(peaks); see Comments.
 * https://oeis.org/A296889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_pits_peaks
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the OEIS sequence A296889.
 *  a(n) = the n-th positive integer whose base‑12 digits have
 *         more “pits” than “peaks”.
 *  (A pit is an index i with d₍i₋₁₎>dᵢ<d₍i₊₁₎;
 *   a peak is an index i with d₍i₋₁₎<dᵢ>d₍i₊₁₎.)
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/count_pits_peaks.h"

/* dynamic cache for computed sequence terms */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static seq_t next_candidate = (seq_t)144;   /* start checking after 144 */

/*  a(n) : index n >= 1   */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined, but harmless */

    /* grow cache if necessary */
    if (cache_len < (size_t)n) {
        size_t needed = n;
        if (needed > cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 256 : cache_cap * 2;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);            /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        int pit, peak;
        while (cache_len < needed) {
            /* test next_candidate */
            count_pits_peaks(next_candidate, &pit, &peak);
            if (pit > peak) {
                cache[cache_len++] = next_candidate;
            }
            next_candidate++;
        }
    }
    return cache[n - 1];
}
