/*
 * A244470  Maximal term in row n of sequence A240388 when regarded as a
 *          triangle.
 * https://oeis.org/A244470
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-3) + a(n-6) for n ≥ 11; Initial values: a(2)=1,
 *            a(3)=1, a(4)=2, a(5)=4, a(6)=6, a(7)=9, a(8)=17, a(9)=25,
 *            a(10)=40 Offset: n ≥ 2
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern seq_t a(idx_t n);  /* pre-declaration for clarity */

seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* undefined below offset */

    /* For n ≤ 10 we can return constants directly. */
    if (n <= 10) {
        static const seq_t base[11] = {
            0, /* unused */
            0, /* unused */
            (seq_t)1,  /* a(2) */
            (seq_t)1,  /* a(3) */
            (seq_t)2,  /* a(4) */
            (seq_t)4,  /* a(5) */
            (seq_t)6,  /* a(6) */
            (seq_t)9,  /* a(7) */
            (seq_t)17, /* a(8) */
            (seq_t)25, /* a(9) */
            (seq_t)40  /* a(10) */
        };
        return base[n];
    }

    /* Dynamic cache for terms a(2) … a(n).  Use realloc to grow. */
    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* Ensure we have room up to n, and also at least indices up to 10
       so that base values can be stored. */
    size_t req = (size_t)n + 1;
    if (req < 11) req = 11;            /* reserve space for indices 2..10 */
    if (req > cap) {
        size_t newcap = req;
        seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!newptr) abort();          /* out of memory */
        /* Initialize newly allocated slots to zero (uncomputed). */
        for (size_t i = cap; i < newcap; ++i) newptr[i] = 0;
        cache = newptr;
        cap = newcap;
    }

    /* Store base values in the cache if they are not already set.
       All sequence values are positive, so zero indicates uninitialized. */
    static const seq_t base[11] = {
        0, 0, (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)4,
        (seq_t)6, (seq_t)9, (seq_t)17, (seq_t)25, (seq_t)40
    };
    for (size_t i = 2; i <= 10; ++i)
        if (cache[i] == 0)
            cache[i] = base[i];

    /* Compute higher terms using the recurrence. */
    for (size_t i = 11; i <= (size_t)n; ++i) {
        if (cache[i] == 0) {  /* uncomputed */
            cache[i] = (seq_t)4 * cache[i - 3] + cache[i - 6];
        }
    }

    return cache[(size_t)n];
}
