/*
 * A307688  a(n) = 2*a(n-1)-2*a(n-2)+a(n-3)+2*a(n-4) with a(0)=a(1)=0, a(2)=2,
 *          a(3)=3.
 * https://oeis.org/A307688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)-2*a(n-2)+a(n-3)+2*a(n-4), initial values:
 *            a(0)=0, a(1)=0, a(2)=2, a(3)=3
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 48, u64 n <= 94, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* dynamic memoisation buffer   */
static size_t capacity = 0;   /* allocated size of cache      */
static idx_t last_cached = 0; /* highest index computed so far */

seq_t a(idx_t n) {
    /* handle small indices directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;

    /* initialise cache once */
    if (cache == NULL) {
        capacity = 16; /* initial allocation, will grow as needed */
        cache    = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* out of memory, return 0 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        last_cached = 3;
    }

    /* grow cache to accommodate required index */
    if ((idx_t)capacity <= n) {
        size_t new_cap = capacity;
        while ((idx_t)new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failed */
        cache = tmp;
        capacity = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = last_cached + 1; i <= n; ++i) {
        seq_t t1 = cache[i-1];
        seq_t t2 = cache[i-2];
        seq_t t3 = cache[i-3];
        seq_t t4 = cache[i-4];

        /* a(i) = 2*t1 - 2*t2 + t3 + 2*t4
         * Compute sum first to avoid negative intermediate */
        seq_t sum = 2*t1 + t3 + 2*t4;
        seq_t result = sum - 2*t2;
        cache[i] = result;
    }

    last_cached = n;
    return cache[n];
}
