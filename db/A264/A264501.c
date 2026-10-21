/*
 * A264501  Number of (n+1)X(3+1) arrays of permutations of 0..n*4+3 with each
 *          element having directed index change -1,1 0,-1 0,1 or 1,0.
 * https://oeis.org/A264501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-2) + 17*a(n-3) + 22*a(n-4) + 5*a(n-5) * - 29*a(n-6)
 *            - 44*a(n-7) - 25*a(n-8) + 11*a(n-9) * + 20*a(n-10) + 11*a(n-11)
 *            - a(n-12) - 2*a(n-13) - a(n-14) * (empirical recurrence, holds
 *            for n ≥ 15). * Initial values: a(1)=3, a(2)=14, a(3)=46,
 *            a(4)=149, a(5)=524, * a(6)=1790, a(7)=6084, a(8)...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* memoised values (a(1) at cache[0]) */
    static size_t cache_len = 0;  /* number of terms stored */
    static size_t cache_cap = 0;  /* allocated capacity */

    /* ---------- initial constants (first 14 terms) ---------- */
    static const seq_t init[14] = {
        (seq_t)3,      (seq_t)14,     (seq_t)46,     (seq_t)149,
        (seq_t)524,    (seq_t)1790,   (seq_t)6084,   (seq_t)20812,
        (seq_t)71127,  (seq_t)242856, (seq_t)829559, (seq_t)2833848,
        (seq_t)9679612, (seq_t)33063445
    };

    /* ---- offset starts at 1 ---- */
    if (n < 1)
        return (seq_t)0;

    /* Ensure allocated space for first n terms */
    while (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)n)
            new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Fill cache with the first 14 terms if not already done */
    if (cache_len < 14) {
        size_t i;
        for (i = cache_len; i < 14; ++i)
            cache[i] = init[i];
        cache_len = 14;
    }

    /* Return directly if n is within the initial window */
    if (n <= 14)
        return init[(size_t)n - 1];

    /* Compute terms a(15) … a(n) using the recurrence */
    idx_t i;
    for (i = (idx_t)cache_len + 1; i <= n; ++i) {
        seq_t term = 0;

        /* Positive contributions */
        term += (seq_t)5   * cache[(size_t)i - 3];  /* a(i-2) */
        term += (seq_t)17  * cache[(size_t)i - 4];  /* a(i-3) */
        term += (seq_t)22  * cache[(size_t)i - 5];  /* a(i-4) */
        term += (seq_t)5   * cache[(size_t)i - 6];  /* a(i-5) */
        term += (seq_t)11  * cache[(size_t)i -10];  /* a(i-9) */
        term += (seq_t)20  * cache[(size_t)i -11];  /* a(i-10) */
        term += (seq_t)11  * cache[(size_t)i -12];  /* a(i-11) */

        /* Negative contributions */
        term -= (seq_t)29  * cache[(size_t)i - 7];  /* a(i-6) */
        term -= (seq_t)44  * cache[(size_t)i - 8];  /* a(i-7) */
        term -= (seq_t)25  * cache[(size_t)i - 9];  /* a(i-8) */
        term -=               cache[(size_t)i -13]; /* a(i-12) */
        term -= (seq_t)2    * cache[(size_t)i -14]; /* a(i-13) */
        term -=               cache[(size_t)i -15]; /* a(i-14) */

        cache[(size_t)i - 1] = term;
    }

    cache_len = (size_t)n;
    return cache[(size_t)n - 1];
}
