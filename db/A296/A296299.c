/*
 * A296299  Dimension of the n-th component of a certain graded Lie algebra.
 * https://oeis.org/A296299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A296299: Dimension of the n‑th component of a certain graded Lie algebra.
    The sequence is the fixed point of the morphism
        μ(1) = 121,  μ(2) = 12221,
    started from the root word 1221.  
    Let b be the infinite word defined by b = 1221 μ(b).  
    Then a(1) = 2, a(2) = 1, and for n ≥ 3 we have a(n) = b[n‑2].
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* External integer types supplied by the build system */
extern seq_t a(idx_t n);

/* Static storage for the generated word b */
static seq_t *bvec = NULL;   /* the sequence b[0], b[1], ...  (values 1 or 2) */
static size_t bsize = 0;     /* current number of elements in bvec */

/* Ensure that bvec contains at least indices 0 … target_index */
static void ensure_bsize(size_t target_index)
{
    if (bsize == 0) {
        /* initialise the root word 1221 */
        bsize = 4;
        bvec = (seq_t *)malloc(bsize * sizeof(seq_t));
        if (!bvec) abort();
        bvec[0] = (seq_t)1;
        bvec[1] = (seq_t)2;
        bvec[2] = (seq_t)2;
        bvec[3] = (seq_t)1;
    }

    /* grow the buffer until it contains the required index */
    while (bsize <= target_index) {
        /* compute length after one expansion */
        size_t newsize = 4;                 /* fixed prefix 1221 */
        for (size_t i = 0; i < bsize; ++i)
            newsize += (bvec[i] == (seq_t)1 ? 3 : 5);

        seq_t *newvec = (seq_t *)malloc(newsize * sizeof(seq_t));
        if (!newvec) abort();

        /* store the fixed prefix 1221 */
        newvec[0] = (seq_t)1;
        newvec[1] = (seq_t)2;
        newvec[2] = (seq_t)2;
        newvec[3] = (seq_t)1;

        /* encode the expansion of each old symbol */
        size_t pos = 4;
        for (size_t i = 0; i < bsize; ++i) {
            if (bvec[i] == (seq_t)1) {
                /* μ(1) = 121 */
                newvec[pos++] = (seq_t)1;
                newvec[pos++] = (seq_t)2;
                newvec[pos++] = (seq_t)1;
            } else {
                /* μ(2) = 12221 */
                newvec[pos++] = (seq_t)1;
                newvec[pos++] = (seq_t)2;
                newvec[pos++] = (seq_t)2;
                newvec[pos++] = (seq_t)2;
                newvec[pos++] = (seq_t)1;
            }
        }

        free(bvec);
        bvec = newvec;
        bsize = newsize;
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1)   /* first term */
        return (seq_t)2;
    if (n == (idx_t)2)   /* second term */
        return (seq_t)1;

    /* for n ≥ 3, a(n) = b[n‑2] */
    size_t target_index = (size_t)(n - (idx_t)2);
    ensure_bsize(target_index);
    return bvec[target_index];
}
