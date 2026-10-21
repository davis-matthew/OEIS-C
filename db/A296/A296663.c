/*
 * A296663  Row sums of A296664.
 * https://oeis.org/A296663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 30, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)=1, a(1)=1, a(2)=4, a(3)=7
   for n≥4:
   a(n) = ( 2*(n-2)*(n^2+n+1)*a(n-1)
           + 4*(n-1)*(n^2-n-5)*a(n-2)
           - 8*(n-2)*(n^2-2)*a(n-3) )
          / ( (n+1)*(n^2-2*n-1) );
*/
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;   /* array of computed a(i) */
    static idx_t cap   = 0;       /* allocated size */
    static idx_t size  = 0;       /* number of computed entries (= last+1) */

    /* first allocation and base terms */
    if (cache == NULL) {
        cap   = 4;                           /* at least up to a(3) */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;         /* allocation failed */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 4;
        cache[3] = 7;
        size = 4;                            /* indices 0..3 are ready */
    }

    /* grow cache if necessary */
    if (n >= cap) {
        while (n >= cap) cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (tmp == NULL) return 0;           /* allocation failed */
        cache = tmp;
    }

    /* if already computed, return immediately */
    if (n < size) return cache[n];

    /* compute values up to n (inclusive) */
    for (idx_t i = size; i <= n; ++i) {
        /* i>=4 here because size ≥4 after first allocation */
        seq_t val;
        if (sizeof(seq_t) <= 8) {         /* small integer type → use 128‑bit intermediates */
            __int128 N   = i;
            __int128 N2  = N * N;
            __int128 term1 = (__int128)2 * (N - 2) * (N2 + N + 1) * (__int128)cache[i-1];
            __int128 term2 = (__int128)4 * (N - 1) * (N2 - N - 5) * (__int128)cache[i-2];
            __int128 term3 = (__int128)8 * (N - 2) * (N2 - 2)      * (__int128)cache[i-3];
            __int128 numer = term1 + term2 - term3;
            __int128 denom = (N + 1) * (N2 - 2 * N - 1);
            val = (seq_t)(numer / denom);   /* division is exact */
        } else {                           /* arbitrary‑precision type */
            seq_t N   = (seq_t)i;
            seq_t N2  = N * N;
            seq_t term1 = (seq_t)2 * (N - (seq_t)2) * (N2 + N + (seq_t)1) * cache[i-1];
            seq_t term2 = (seq_t)4 * (N - (seq_t)1) * (N2 - N - (seq_t)5) * cache[i-2];
            seq_t term3 = (seq_t)8 * (N - (seq_t)2) * (N2 - (seq_t)2)     * cache[i-3];
            seq_t numer = term1 + term2 - term3;
            seq_t denom = (N + (seq_t)1) * (N2 - (seq_t)2 * N - (seq_t)1);
            val = numer / denom;          /* exact division for integer seq_t */
        }
        cache[i] = val;
    }

    size = n + 1;
    return cache[n];
}
