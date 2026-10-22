/*
 * A337120  Factor complexity (number of subwords of length n) of the regular
 *          paperfolding sequence (A014577), and all generalized paperfolding
 *          sequences.
 * https://oeis.org/A337120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A337120: Factor complexity of the regular paperfolding sequence.
   Initial terms: a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=12, a(5)=18,
   a(6)=23, a(7)=28, a(8)=32.  For n≥9 the recurrence holds:
   a(n)=2*a(n-1)-a(n-2). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to allow arbitrary call order and keep O(1) average per call. */
    static seq_t *cache = NULL;   /* array of computed values */
    static size_t cap   = 0;       /* allocated capacity (in terms) */
    static size_t len   = 0;       /* next free index (size of valid data) */

    /* Resize cache if we need to store up to index n. */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0; /* out‑of‑memory; fallback to 0 */
        /* copy old values if any */
        for (size_t i = 0; i < len; ++i) new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cap   = new_cap;
    }

    /* Compute any missing terms up to n. */
    if (len <= (size_t)n) {
        for (size_t i = len; i <= (size_t)n; ++i) {
            seq_t val;
            switch (i) {
                case 0:  val = 1;  break;
                case 1:  val = 2;  break;
                case 2:  val = 4;  break;
                case 3:  val = 8;  break;
                case 4:  val = 12; break;
                case 5:  val = 18; break;
                case 6:  val = 23; break;
                case 7:  val = 28; break;
                case 8:  val = 32; break;
                default: val = 2 * cache[i-1] - cache[i-2];
            }
            cache[i] = val;
        }
        len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
