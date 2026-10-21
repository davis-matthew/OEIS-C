/*
 * A281466  Number of nX5 0..1 arrays with no element equal to more than two
 *          of its horizontal, diagonal or antidiagonal neighbors and with new
 *          values introduced in order 0 sequentially upwards.
 * https://oeis.org/A281466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 27, u64 n <= 57, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:
   a(n) = 2*a(n-1) +3*a(n-3) -4*a(n-4) -3*a(n-5)
          -a(n-6) -a(n-7) +8*a(n-8) -3*a(n-10)    for n>15.
   Initial terms:
   a(1)=16, a(2)=78, a(3)=57, a(4)=130, a(5)=278, a(6)=534,
   a(7)=1106, a(8)=2362, a(9)=4901, a(10)=10240, a(11)=21532,
   a(12)=45098, a(13)=94623, a(14)=198599, a(15)=416401.
   (Offset 1 – a(1) is the first term.)  */
#include <stdlib.h>

static const seq_t base[15] = {
    (seq_t)16,   (seq_t)78,   (seq_t)57,   (seq_t)130,
    (seq_t)278,  (seq_t)534,  (seq_t)1106, (seq_t)2362,
    (seq_t)4901, (seq_t)10240, (seq_t)21532, (seq_t)45098,
    (seq_t)94623, (seq_t)198599, (seq_t)416401
};

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                 /* undefined for n<1 */

    /* static cache for memoisation */
    static seq_t *cache = NULL;                 /* array of a(1),a(2),... */
    static idx_t cached_len = 0;                /* number of stored terms */
    static size_t cache_cap = 0;                /* allocated elements */

    /* initialise cache once with the known 15 terms */
    if (!cache) {
        cache_cap = 16;                         /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 15; ++i) cache[i] = base[i];
        cached_len = 15;
    }

    /* grow storage if necessary */
    if ((size_t)n > cache_cap) {
        cache_cap = (size_t)n;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* return memoised value if already computed */
    if (n <= cached_len) return cache[(size_t)(n - 1)];

    /* compute new terms up to n (inclusive) */
    for (idx_t i = cached_len + 1; i <= n; ++i) {
        /* i is 1‑based index of the term a(i) */
        size_t j = (size_t)(i - 1);            /* array index of a(i) */
        seq_t term = 0;

        term += (seq_t)2 * cache[j - 1];
        term += (seq_t)3 * cache[j - 3];
        term -= (seq_t)4 * cache[j - 4];
        term -= (seq_t)3 * cache[j - 5];
        term -=       cache[j - 6];
        term -=       cache[j - 7];
        term += (seq_t)8 * cache[j - 8];
        term -= (seq_t)3 * cache[j -10];

        cache[j] = term;
    }

    cached_len = n;
    return cache[(size_t)(n - 1)];
}
