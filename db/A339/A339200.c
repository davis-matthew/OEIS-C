/*
 * A339200  Number of (undirected) Hamiltonian cycles on the n X 3 king graph.
 * https://oeis.org/A339200
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the empirical generating function:
   a(2)=4, a(3)=16, a(4)=120, a(5)=744, a(6)=4922.
   For n>=7 : a(n)=4*a(n-1)+15*a(n-2)+8*a(n-3)+6*a(n-4) */
#include <stddef.h>

seq_t a(idx_t n) {
    /* Handle indices below the offset */
    if (n < 2) return (seq_t)0;

    /* Use a static array big enough for n up to about 10000.
       It is more than sufficient for the test range.   */
    enum { MAX_STATIC = 20000 };
    static seq_t static_cache[MAX_STATIC + 1];
    static idx_t cache_len = 0;
    static int initialized = 0;

    if (!initialized) {
        /* Initialize the base values.  The whole array is zero‑initialised
           by the compiler for static objects. */
        static_cache[2] = (seq_t)4;
        static_cache[3] = (seq_t)16;
        static_cache[4] = (seq_t)120;
        static_cache[5] = (seq_t)744;
        static_cache[6] = (seq_t)4922;
        cache_len = 6;
        initialized = 1;
    }

    if (n <= cache_len) {
        return static_cache[n];
    }

    /* Compute missing terms up to n.  The recurrence is valid for n>=7. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t term;
        if (i >= 7) {
            term = (seq_t)4 * static_cache[i-1] +
                   (seq_t)15 * static_cache[i-2] +
                   (seq_t)8 * static_cache[i-3] +
                   (seq_t)6 * static_cache[i-4];
        } else {
            /* For i==7 the same formula holds, but we keep the
               guard for clarity.  All higher i will use the formula. */
            term = (seq_t)4 * static_cache[i-1] +
                   (seq_t)15 * static_cache[i-2] +
                   (seq_t)8 * static_cache[i-3] +
                   (seq_t)6 * static_cache[i-4];
        }
        static_cache[i] = term;
    }
    cache_len = n;
    return static_cache[n];
}
