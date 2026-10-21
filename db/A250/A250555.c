/*
 * A250555  Number of length n+2 0..2 arrays with the sum of second
 *          differences multiplied by some arrangement of +-1 equal to zero.
 * https://oeis.org/A250555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2) - 17*a(n-3) - 3*a(n-4) + 24*a(n-5) +
 *            16*a(n-6) - 13*a(n-7) - 26*a(n-8) + 2*a(n-9) + 12*a(n-10) for n
 *            > 15 Initial values (offset 1): a(1)=5, a(2)=25, a(3)=83,
 *            a(4)=297, a(5)=989, a(6)=3113, a(7)=9611, a(8)=29257,
 *            a(9)=88503, a(10)=266769, a(11)=802699, a(12)=2412545, a(...
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values of the sequence.  cache[0] == a(1). */
static seq_t *cache = NULL;
static idx_t used = 0;   /* number of terms currently stored in cache */

/* Ensure the cache contains at least n terms (indices 1..n). */
static void ensure(idx_t n)
{
    if (used >= n) return;

    /* Allocate or grow the cache. */
    idx_t need = n;
    if (cache == NULL) {
        /* Initialise with the first 16 slots (actually 15 needed). */
        need = n >= 16 ? n : 16;
        cache = (seq_t *)calloc((size_t)need, sizeof(seq_t));
        /* Initial terms a(1)..a(15). */
        static const seq_t init[15] = {
            (seq_t)5,          /* a(1) */
            (seq_t)25,         /* a(2) */
            (seq_t)83,         /* a(3) */
            (seq_t)297,        /* a(4) */
            (seq_t)989,        /* a(5) */
            (seq_t)3113,       /* a(6) */
            (seq_t)9611,       /* a(7) */
            (seq_t)29257,      /* a(8) */
            (seq_t)88503,      /* a(9) */
            (seq_t)266769,     /* a(10) */
            (seq_t)802699,     /* a(11) */
            (seq_t)2412545,    /* a(12) */
            (seq_t)7246311,    /* a(13) */
            (seq_t)21755889,   /* a(14) */
            (seq_t)65301163    /* a(15) */
        };
        for (idx_t i = 0; i < 15; ++i) cache[i] = init[i];
        used = 15;
    }
    else if (need > used) {
        cache = (seq_t *)realloc(cache, (size_t)need * sizeof(seq_t));
    }

    /* Compute terms from index used up to n-1 (0‑based).  The recurrence
       uses the previous 10 terms, so we need used >= 15 before computing
       any new term.  Since used is 15 on first entry, we can start at i = used. */
    for (idx_t i = used; i < n; ++i) {
        /* i corresponds to a(i+1).  The recurrence requires a(i), a(i-1), …, a(i-9). */
        cache[i] = (seq_t)4  * cache[i-1]
                 + (seq_t)2  * cache[i-2]
                 - (seq_t)17 * cache[i-3]
                 - (seq_t)3  * cache[i-4]
                 + (seq_t)24 * cache[i-5]
                 + (seq_t)16 * cache[i-6]
                 - (seq_t)13 * cache[i-7]
                 - (seq_t)26 * cache[i-8]
                 + (seq_t)2  * cache[i-9]
                 + (seq_t)12 * cache[i-10];
    }
    used = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined offset behaviour */
    if (n <= 15) {                      /* small indexes use the stored constants */
        static const seq_t init[15] = {
            (seq_t)5,          /* a(1) */
            (seq_t)25,         /* a(2) */
            (seq_t)83,         /* a(3) */
            (seq_t)297,        /* a(4) */
            (seq_t)989,        /* a(5) */
            (seq_t)3113,       /* a(6) */
            (seq_t)9611,       /* a(7) */
            (seq_t)29257,      /* a(8) */
            (seq_t)88503,      /* a(9) */
            (seq_t)266769,     /* a(10) */
            (seq_t)802699,     /* a(11) */
            (seq_t)2412545,    /* a(12) */
            (seq_t)7246311,    /* a(13) */
            (seq_t)21755889,   /* a(14) */
            (seq_t)65301163    /* a(15) */
        };
        return init[n-1];
    }
    ensure(n);
    return cache[n-1];   /* cache[0]==a(1) */
}
