/*
 * A347732  Number of compositions (ordered partitions) of n into at most 4
 *          triangular numbers.
 * https://oeis.org/A347732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into at most 4 triangular
 *            numbers
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#if defined(__cplusplus)
#  define ALLOC_SEQ(n)  new seq_t[(n)]()
#  define FREE_SEQ(p)   delete[] (p)
#else
#  define ALLOC_SEQ(n)  ((seq_t*)calloc((n), sizeof(seq_t)))
#  define FREE_SEQ(p)   free((p))
#endif

seq_t a(idx_t n)
{
    /* static state */
    static seq_t *c1 = NULL, *c2 = NULL, *c3 = NULL, *c4 = NULL, *acache = NULL;
    static idx_t computed_up_to = 0;

    /* n == 0: the empty composition */
    if (n == 0)
        return (seq_t)1;

    /* Recompute if a larger n is requested */
    if (n > computed_up_to) {
        size_t limit = (size_t)n;
        size_t cap   = limit + 1;   /* array size */

        /* free previous storage */
        if (c1)     FREE_SEQ(c1);
        if (c2)     FREE_SEQ(c2);
        if (c3)     FREE_SEQ(c3);
        if (c4)     FREE_SEQ(c4);
        if (acache) FREE_SEQ(acache);

        /* allocate zero‑initialized arrays */
        c1     = ALLOC_SEQ(cap);
        c2     = ALLOC_SEQ(cap);
        c3     = ALLOC_SEQ(cap);
        c4     = ALLOC_SEQ(cap);
        acache = ALLOC_SEQ(cap);

        /* compute triangular numbers ≤ limit */
        size_t *tri = (size_t*)malloc(cap * sizeof(size_t));
        size_t tcnt = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t t = k * (k + 1) / 2;
            if (t > limit) break;
            tri[tcnt++] = (size_t)t;
        }

        /* compositions with exactly 1 part */
        for (size_t i = 0; i < tcnt; ++i)
            c1[tri[i]] = (seq_t)1;

        /* compositions with 2 parts: convolution of c1 with triangular */
        for (size_t i = 0; i < tcnt; ++i) {
            size_t tv = tri[i];
            for (size_t sum = tv; sum <= limit; ++sum)
                c2[sum] += c1[sum - tv];
        }

        /* compositions with 3 parts */
        for (size_t i = 0; i < tcnt; ++i) {
            size_t tv = tri[i];
            for (size_t sum = tv; sum <= limit; ++sum)
                c3[sum] += c2[sum - tv];
        }

        /* compositions with 4 parts */
        for (size_t i = 0; i < tcnt; ++i) {
            size_t tv = tri[i];
            for (size_t sum = tv; sum <= limit; ++sum)
                c4[sum] += c3[sum - tv];
        }

        /* total with at most 4 parts (and the empty composition) */
        acache[0] = (seq_t)1;                 /* empty composition for n = 0 */
        for (size_t i = 1; i <= limit; ++i)
            acache[i] = c1[i] + c2[i] + c3[i] + c4[i];

        /* cleanup */
        free(tri);
        computed_up_to = n;
    }

    return acache[n];
}
