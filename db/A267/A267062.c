/*
 * A267062  Coefficient of x^3 in the minimal polynomial of the continued
 *          fraction [1^n,sqrt(2)+sqrt(3),1,1,...], where 1^n means n ones.
 * https://oeis.org/A267062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 none, i16 n <= 1, i32 n <= 4, i64 n <= 10, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A267062
 *
 *  a(n) = 34*a(n-1) + 714*a(n-2) - 4641*a(n-3) - 12376*a(n-4)
 *          + 12376*a(n-5) + 4641*a(n-6) - 714*a(n-7) - 34*a(n-8) + a(n-9)
 *  a(0) = 212,  a(1) = -5760,  a(2) = -165852,
 *  a(3) = -10501476,  a(4) = -449827456,
 *  a(5) = -21948311748,  a(6) = -1016699956620,
 *  a(7) = -48023357086272,  a(8) = -2251419462422716
 *
 *  The function computes any term a(n) for n >= 0, memoising previously
 *  computed values so that repeated calls are fast.  The first nine values
 *  are stored as constants and used as the seed of the 9‑term linear
 *  recurrence.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Seed values (first nine terms of the sequence).  */
    static const seq_t seed[9] = {
        (seq_t)212,
        (seq_t)-5760,
        (seq_t)-165852,
        (seq_t)-10501476,
        (seq_t)-449827456,
        (seq_t)-21948311748,
        (seq_t)-1016699956620,
        (seq_t)-48023357086272,
        (seq_t)-2251419462422716
    };

    /* Memoisation cache – grows lazily.  */
    static seq_t *cache = NULL;          /* array of computed terms */
    static size_t  cache_cap = 0;        /* allocated size */
    static idx_t   max_computed = (idx_t)-1;   /* largest n with a(n) stored */

    /* Initialise cache with the seed values.  */
    if (cache == NULL) {
        cache_cap = 9;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* fallback on OOM */
        for (idx_t i = 0; i < 9; ++i)
            cache[i] = seed[i];
        max_computed = 8;   /* indices 0..8 are valid */
    }

    /* Ensure the cache is large enough for index n.  */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap = new_cap * 2 + 1;  /* avoid infinite loop if new_cap==0 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* fallback on OOM */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If the requested term is already computed, return it.  */
    if (n <= max_computed)
        return cache[n];

    /* Compute missing terms up to n using the recurrence.  */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t v =  (seq_t)34  * cache[i-1] +
                  (seq_t)714 * cache[i-2] -
                  (seq_t)4641 * cache[i-3] -
                  (seq_t)12376 * cache[i-4] +
                  (seq_t)12376 * cache[i-5] +
                  (seq_t)4641  * cache[i-6] -
                  (seq_t)714  * cache[i-7] -
                  (seq_t)34   * cache[i-8]  +
                  cache[i-9];
        cache[i] = v;
    }

    max_computed = n;
    return cache[n];
}
