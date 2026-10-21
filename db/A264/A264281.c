/*
 * A264281  Number of (n+1) X (4+1) arrays of permutations of 0..n*5+4 with
 *          each element having directed index change 0,0 0,1 1,0 or -1,-2.
 * https://oeis.org/A264281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 2*a(n-2) - 2*a(n-3) - 6*a(n-4) + a(n-5) +
 *            3*a(n-6) Initial values: a(1)=16, a(2)=102, a(3)=675, a(4)=4484,
 *            a(5)=29742, a(6)=197283
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 1) return 0; /* undefined for n < 1, but return 0 */

    /* Allocate cache for computed values, index 0..max_n. 0-th entry unused. */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    size_t needed = (size_t)n + 6;           /* +6 for recurrence safety */
    if (cap < needed) {
        size_t newcap = cap ? cap << 1 : 64;
        if (newcap < needed) newcap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        cap = newcap;
    }

    /* Compute values up to n.  Use static typed loop. */
    for (size_t i = 1; i <= (size_t)n; ++i) {
        if (i <= 6 && cache[i] != 0) continue;   /* already initialized */
        if (i == 1) { cache[i] = 16; continue; }
        if (i == 2) { cache[i] = 102; continue; }
        if (i == 3) { cache[i] = 675; continue; }
        if (i == 4) { cache[i] = 4484; continue; }
        if (i == 5) { cache[i] = 29742; continue; }
        if (i == 6) { cache[i] = 197283; continue; }

        /* Use 128‑bit intermediate arithmetic.  Works for built‑in integer
           types and, if the build supplies a larger unsigned type, also
           promotes correctly. */
#ifdef __cplusplus
        /* In C++ we can rely on operator overloading; but for safety we
           still perform the multiplication in __uint128_t when the underlying
           type fits in 64 bits. */
        if (sizeof(seq_t) <= 8) {
            unsigned __int128 temp = (unsigned __int128)cache[i-1] * 7
                                  - (unsigned __int128)cache[i-2] * 2
                                  - (unsigned __int128)cache[i-3] * 2
                                  - (unsigned __int128)cache[i-4] * 6
                                  + (unsigned __int128)cache[i-5]
                                  + (unsigned __int128)cache[i-6] * 3;
            cache[i] = (seq_t)temp;
        } else {
            cache[i] = (seq_t)7 * cache[i-1]
                     - (seq_t)2 * cache[i-2]
                     - (seq_t)2 * cache[i-3]
                     - (seq_t)6 * cache[i-4]
                     + cache[i-5]
                     + (seq_t)3 * cache[i-6];
        }
#else
        unsigned __int128 temp = (unsigned __int128)cache[i-1] * 7
                              - (unsigned __int128)cache[i-2] * 2
                              - (unsigned __int128)cache[i-3] * 2
                              - (unsigned __int128)cache[i-4] * 6
                              + (unsigned __int128)cache[i-5]
                              + (unsigned __int128)cache[i-6] * 3;
        cache[i] = (seq_t)temp;
#endif
    }

    return cache[(size_t)n];
}
