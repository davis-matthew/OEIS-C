/*
 * A264671  Number of (n+1)X(3+1) arrays of permutations of 0..n*4+3 with each
 *          element having directed index change -1,-1 1,0 -1,-2 -2,-2 or 0,1.
 * https://oeis.org/A264671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A264671
 *
 * Recurrence (for n > 15):
 *   a(n) = 2*a(n-1) + 6*a(n-2) + 10*a(n-3) + 6*a(n-5) + 11*a(n-6) +
 *          4*a(n-7) + 22*a(n-8) + 21*a(n-9) - 4*a(n-10) - 10*a(n-11) -
 *          10*a(n-12) - 2*a(n-13) + a(n-15)
 * Initial values (1‑based indexing):
 *   a(1)=1,  a(2)=10,  a(3)=34,  a(4)=145,  a(5)=603,
 *   a(6)=2417, a(7)=9976, a(8)=40825, a(9)=166985,
 *   a(10)=684253, a(11)=2801318, a(12)=11470702,
 *   a(13)=46972259, a(14)=192337615, a(15)=787587465
 */
#include <stdlib.h>

/* Compute the n-th term of A264671, with 1‑based index. */
seq_t a(idx_t n)
{
    /* Constants for the first 15 terms (n = 1 .. 15) */
    static const seq_t init[15] = {
        1ULL,          10ULL,          34ULL,          145ULL,
        603ULL,        2417ULL,        9976ULL,        40825ULL,
        166985ULL,     684253ULL,     2801318ULL,    11470702ULL,
        46972259ULL,   192337615ULL,   787587465ULL
    };

    /* The cache holds computed values.
       cache[0] is unused to keep the indices 1‑based. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* Number of allocated slots, including index 0 */

    /* Return a known initial value directly. */
    if (n <= 15) {
        return init[(idx_t)(n - 1)];
    }

    /* Lazy allocation of cache and copy of the initial values. */
    if (cache == NULL) {
        cache_len = 16;                         /* indices 0 .. 15 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (cache == NULL) return 0;            /* allocation failed */
        cache[0] = 0;                           /* unused placeholder */
        for (idx_t i = 1; i <= 15; ++i) {
            cache[i] = init[(idx_t)(i - 1)];
        }
    }

    /* Grow cache if needed and compute new terms up to n. */
    if ((idx_t)(n + 1) > cache_len) {           /* we need indices up to n inclusive */
        idx_t new_len = (idx_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (tmp == NULL) return 0;              /* allocation failed */
        cache = tmp;
        /* Compute missing terms. */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t pos = 0;
            seq_t neg = 0;

            pos += 2      * cache[i - 1];
            pos += 6      * cache[i - 2];
            pos += 10     * cache[i - 3];
            pos += 6      * cache[i - 5];
            pos += 11     * cache[i - 6];
            pos += 4      * cache[i - 7];
            pos += 22     * cache[i - 8];
            pos += 21     * cache[i - 9];
            pos +=         cache[i - 15];

            neg += 4      * cache[i -10];
            neg += 10     * cache[i -11];
            neg += 10     * cache[i -12];
            neg += 2      * cache[i -13];

            cache[i] = pos - neg;
        }
        cache_len = new_len;
    }

    return cache[n];
}
