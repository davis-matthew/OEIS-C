/*
 * A377265  Numbers n such that 2*n contains one more 1 than does n in its
 *          decimal representation.
 * https://oeis.org/A377265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 6286, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A377265
 *  a(n) is the nth positive integer x such that
 *  the number of decimal digit '1' in 2*x is exactly one more
 *  than in x.
 *  (Sequence defined by a search; no closed formula.)
 */

#include <stdlib.h>

static unsigned int count_ones(seq_t x)
{
    unsigned int cnt = 0;
    while (x > (seq_t)0) {
        if (x % (seq_t)10 == (seq_t)1)
            cnt++;
        x /= (seq_t)10;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; return 0 for the unused n==0 */
    if (n == 0)
        return (seq_t)0;

    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cached  = 0;   /* number of terms stored */
    static size_t capacity = 0;  /* allocated capacity (in terms) */
    static seq_t last_tested = (seq_t)0; /* last candidate examined */

    /* ensure the cache can hold at least n terms */
    if (capacity < (size_t)n) {
        size_t new_cap = capacity ? capacity * 2 : 64;
        if (new_cap < (size_t)n)
            new_cap = (size_t)n;
        seq_t *tmp = (seq_t *) realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) /* allocation failure */
            return (seq_t)0;
        cache = tmp;
        capacity = new_cap;
    }

    /* generate missing terms */
    while (cached < (size_t)n) {
        seq_t x = last_tested + (seq_t)1;
        last_tested = x;
        unsigned int c1 = count_ones(x);
        unsigned int c2 = count_ones(x * (seq_t)2);
        if (c2 == c1 + 1)
            cache[cached++] = x;
    }

    return cache[n - 1];
}
