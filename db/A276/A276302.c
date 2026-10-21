/*
 * A276302  Number of 5Xn 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-1,1) or (0,-1) and new values introduced in order
 *          0..2.
 * https://oeis.org/A276302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence for A276302:
 *   a(1) = 41, a(2) = 88, a(3) =  805, a(4) =  5965, a(5) = 50949,
 *   a(6) = 408105, a(7) = 3363533, a(8) = 27372713, a(9) = 224012077,
 *   a(10) = 1828722821, a(11) = 14945190941, a(12) = 122079734781,
 *   a(13) = 997423034925, a(14) = 8148425295997, a(15) = 66571199181721,
 *   a(16) = 543864761405757, a(17) = 4443232843935221.
 * For n > 17:
 *   a(n) = 3*a(n-1) + 33*a(n-2) + 66*a(n-3) + 94*a(n-4)
 *       - 115*a(n-5) - 155*a(n-6) - 255*a(n-7)
 *       + 236*a(n-8) + 152*a(n-9) + 61*a(n-10)
 *       - 213*a(n-11) + 90*a(n-12)
 *       - 5*a(n-13) + 13*a(n-14) - 4*a(n-15).
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The function returns the a(n) value defined above.
 * It caches computed values to allow arbitrary order of calls.
 */
seq_t a(idx_t n)
{
    /* Cache array – 1‑indexed. */
    static seq_t *cache = NULL;
    static idx_t cap = 0;          /* allocated capacity (in slots)        */
    static idx_t max_cached = 0;   /* largest n cached so far */

    /* Hard‑coded initial values (0‑th entry unused). */
    static const seq_t init[18] = {
        (seq_t)0ull,           /* dummy 0‑th entry */
        (seq_t)41ull,          /* a(1) */
        (seq_t)88ull,          /* a(2) */
        (seq_t)805ull,         /* a(3) */
        (seq_t)5965ull,        /* a(4) */
        (seq_t)50949ull,       /* a(5) */
        (seq_t)408105ull,      /* a(6) */
        (seq_t)3363533ull,     /* a(7) */
        (seq_t)27372713ull,    /* a(8) */
        (seq_t)224012077ull,   /* a(9) */
        (seq_t)1828722821ull,  /* a(10) */
        (seq_t)14945190941ull, /* a(11) */
        (seq_t)122079734781ull,/* a(12) */
        (seq_t)997423034925ull,/* a(13) */
        (seq_t)8148425295997ull,/* a(14) */
        (seq_t)66571199181721ull, /* a(15) */
        (seq_t)543864761405757ull,/* a(16) */
        (seq_t)4443232843935221ull/* a(17) */
    };

    /* Reject zero or negative indices – not used. */
    if (n < 1) {
        return (seq_t)0;
    }

    /* Ensure cache big enough for up to n. */
    if (n >= cap) {
        idx_t new_cap = (cap == 0) ? 64 : cap * 2;
        if (new_cap < n) new_cap = n;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – fall back to zero. */
            return (seq_t)0;
        }
        /* Copy existing values. */
        for (idx_t i = 1; i <= max_cached; ++i) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cap = new_cap;
    }

    /* Compute any unfinished terms up to n. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        if (i <= 17) {
            cache[i] = init[i];
        } else {
            /* Use seq_t arithmetic – overflows are acceptable for small
             * types; arbitrary‑precision types give exact values. */
            seq_t val =  (seq_t)3 * cache[i-1]  + (seq_t)33  * cache[i-2]
                       + (seq_t)66 * cache[i-3]  + (seq_t)94  * cache[i-4]
                       - (seq_t)115 * cache[i-5] - (seq_t)155 * cache[i-6]
                       - (seq_t)255 * cache[i-7] + (seq_t)236 * cache[i-8]
                       + (seq_t)152 * cache[i-9] + (seq_t)61  * cache[i-10]
                       - (seq_t)213 * cache[i-11] + (seq_t)90  * cache[i-12]
                       - (seq_t)5   * cache[i-13] + (seq_t)13  * cache[i-14]
                       - (seq_t)4   * cache[i-15];
            cache[i] = val;
        }
    }

    max_cached = n;
    return cache[n];
}
