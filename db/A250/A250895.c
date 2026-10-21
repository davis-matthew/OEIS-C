/*
 * A250895  Number of (n+1) X (5+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 60*a(n-2) + 162*a(n-3) - 255*a(n-4) +
 *            234*a(n-5) - 116*a(n-6) + 24*a(n-7) for n > 9. Initial terms:
 *            a(1)=3673, a(2)=11029, a(3)=33821, a(4)=102789, a(5)=305429,
 *            a(6)=890199, a(7)=2553537, a(8)=7243359, a(9)=20421217
 * Range:     u8 none, u16 n <= 3, u32 n <= 14, u64 n <= 34, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t pre[10] = {
    0ULL,                /* a(0) unused */
    3673ULL, 11029ULL, 33821ULL, 102789ULL,
    305429ULL, 890199ULL, 2553537ULL,
    7243359ULL, 20421217ULL
};

static seq_t *cache = NULL;     /* allocated array indexed 1..N */
static idx_t cache_cap = 0;     /* allocated capacity (not counting index 0) */
static idx_t cache_len = 0;     /* highest index that has been computed */

/* enlarge cache array to hold at least min_cap elements (1..min_cap) */
static void ensure_capacity(idx_t min_cap) {
    if (cache_cap >= min_cap) return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    /* Return precomputed values for small indices */
    if (n <= 9) return pre[n];

    /* Initialize cache up to index 9 on first use */
    if (cache_len < 9) {
        ensure_capacity(9);
        for (idx_t i = 1; i <= 9; ++i) cache[i] = pre[i];
        cache_len = 9;
    }

    /* Extend cache to n if necessary */
    if (n > cache_len) {
        ensure_capacity(n);
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t val =
                ((seq_t)12)  * cache[i-1]
              - ((seq_t)60)  * cache[i-2]
              + ((seq_t)162) * cache[i-3]
              - ((seq_t)255) * cache[i-4]
              + ((seq_t)234) * cache[i-5]
              - ((seq_t)116) * cache[i-6]
              + ((seq_t)24)  * cache[i-7];
            cache[i] = val;
        }
        cache_len = n;
    }

    return cache[n];
}
