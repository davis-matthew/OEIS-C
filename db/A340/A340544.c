/*
 * A340544  Numbers from A340131 that are not multiples of 3.
 * https://oeis.org/A340544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n): Numbers from A340131 that are not multiples of 3.
    The function enumerates all reduced ternary codes of Motzkin
    paths that start with an up step (digit 1) and end with a down step
    (digit 2).  For a fixed length L the path is encoded as a L‑digit
    ternary number w₀…w_{L-1} with w₀ = 1, w_{L-1} = 2.
    The path never goes below height 0; height 0 may be reached
    in interior positions but the height before the final down step
    must be 1 (so that the last step brings the height to 0).
    All such numbers are listed in increasing length, which yields
    strictly increasing integer values.  The array cache stores the
    terms in the order they are generated, allowing any order of
    calls to `a`.  The implementation is O(n) overall for generating
    the first n terms.  It works for all supported seq_t types,
    including arbitrary‑precision builds.  */

#include <stdlib.h>

static seq_t *cache       = NULL;   /* stored terms   */
static size_t cache_cap   = 0;      /* capacity of cache   */
static size_t cache_len   = 0;      /* number of stored terms   */
static idx_t  next_len    = 2;      /* next ternary length to generate  */

/* Ensure that the cache can hold “need” terms.  */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
#ifdef __cplusplus
    cache = reinterpret_cast<seq_t*>(realloc(cache, new_cap * sizeof(seq_t)));
#else
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
    cache_cap = new_cap;
}

/* Recursive enumeration for a fixed ternary length L.
   pos   – current index to set (0‑based).
   val   – value of the digits already fixed.
   height – current path height after the digits up to pos-1.
   L     – total length. */
static void rec_gen(idx_t pos, seq_t val, int height, idx_t L)
{
    if (pos == L - 1) {           /* final digit is always 2 */
        if (height == 1) {        /* height must be 1 before final down */
            seq_t finalVal = val * 3 + 2;   /* last digit 2 */
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = finalVal;
        }
    } else {                       /* interior digit 0,1,2 */
        for (int d = 0; d < 3; ++d) {
            int newH = height + (d == 1 ? 1 : (d == 2 ? -1 : 0));
            if (newH < 0) continue;          /* path cannot go below 0 */
            seq_t newVal = val * 3 + d;
            rec_gen(pos + 1, newVal, newH, L);
        }
    }
}

/* Generate all terms of a given ternary length L. */
static void generate_len(idx_t L)
{
    if (L == 2) {         /* only the string 12 */
        seq_t v = 1;
        v = v * 3 + 2;     /* 12_3 = 5 */
        ensure_capacity(cache_len + 1);
        cache[cache_len++] = v;
    } else {
        rec_gen(1, (seq_t)1, 1, L);   /* start with digit 1, height 1 */
    }
}

/* Public interface required by the build system. */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        generate_len(next_len++);
    }
    return cache[n - 1];
}
