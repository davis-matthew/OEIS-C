/*
 * A266429  Number of 3 X n binary arrays with rows and columns
 *          lexicographically nondecreasing and column sums nondecreasing.
 * https://oeis.org/A266429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=4, a(2)=13, a(3)=39, a(4)=106, a(5)=259, a(6)=574,
 *            a(7)=1170. For n>7:
 *            a(n)=7*a(n-1)-21*a(n-2)+35*a(n-3)-35*a(n-4)+21*a(n-5)-7*a(n-6)+a(n-7).
 *            The implementation uses a forward‑difference scheme that avoids
 *            negative intermediate values
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 106, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state */
    static seq_t *cache = NULL;   /* 1‑based cache (cache[0] unused) */
    static idx_t  cache_size = 0;
    static seq_t diff[7];         /* forward differences at the last cached index */

    if (n == (idx_t)0)              /* offset 1, values below not required */
        return (seq_t)0;

    /* ---------- initialisation of first 7 terms and difference array ---------- */
    if (cache_size < 7) {
        /* allocate cache for indices 0..7 (index 0 unused) */
        cache = (seq_t *)malloc((7ULL + 1ULL) * sizeof(seq_t));

        /* first seven values */
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)13;
        cache[3] = (seq_t)39;
        cache[4] = (seq_t)106;
        cache[5] = (seq_t)259;
        cache[6] = (seq_t)574;
        cache[7] = (seq_t)1170;

        /* initialise diff array to state at index 1 */
        seq_t temp[7];
        for (idx_t i = 0; i < 7; ++i)
            temp[i] = cache[i + 1];
        diff[0] = cache[1];
        for (int level = 1; level <= 6; ++level) {
            for (idx_t i = 0; i < 7 - level; ++i)
                temp[i] = temp[i + 1] - temp[i];
            diff[level] = temp[0];
        }

        /* propagate the algorithm to produce cache[2]..cache[7] and
           to bring diff to the state at index 7. */
        for (idx_t idx = 2; idx <= 7; ++idx) {
            seq_t old[7];
            for (int k = 0; k < 7; ++k) old[k] = diff[k];

            seq_t next = old[0] + old[1];
            diff[5] = old[5] + old[6];
            diff[4] = old[4] + old[5];
            diff[3] = old[3] + old[4];
            diff[2] = old[2] + old[3];
            diff[1] = old[1] + old[2];
            diff[0] = next;

            cache[idx] = next;
        }
        cache_size = 7;
    }

    /* ---------- compute values if needed ---------- */
    if (n > cache_size) {
        /* enlarge cache to hold up to index n */
        cache = (seq_t *)realloc(cache, (n + 1ULL) * sizeof(seq_t));

        for (idx_t idx = cache_size + 1; idx <= n; ++idx) {
            /* compute next value using current forward differences */
            seq_t old[7];
            for (int k = 0; k < 7; ++k) old[k] = diff[k];

            seq_t next = old[0] + old[1];
            diff[5] = old[5] + old[6];
            diff[4] = old[4] + old[5];
            diff[3] = old[3] + old[4];
            diff[2] = old[2] + old[3];
            diff[1] = old[1] + old[2];
            diff[0] = next;

            cache[idx] = next;
        }
        cache_size = n;
    }

    return cache[n];
}
