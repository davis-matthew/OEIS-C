/*
 * A322741  Sorted list of 84 distinct triangle areas corresponding to the
 *          unique solution to the problem of placing 9 points on a grid
 *          rectangle of minimal area, such that all triangles formed by any 3
 *          of the points have distinct areas > 0.
 * https://oeis.org/A322741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A322741: sorted doubled areas of the 84 distinct triangles
   formed by the unique set of 9 grid points in the optimal 18×11 rectangle.
   The values are obtained by enumerating all C(9,3) triangles,
   computing their doubled area
   |x1(y2−y3)+x2(y3−y1)+x3(y1−y2)|, storing the 84 distinct results,
   and sorting them in ascending order. */

#include <stdlib.h>
#include <stdbool.h>

static const int xs[9] = {0, 1, 2, 3, 4, 12, 17, 18, 18};
static const int ys[9] = {3, 9, 0,10,11, 2, 11, 1, 4};

static int compare_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    return (*a > *b) - (*a < *b);          /* -1, 0, 1 */
}

static seq_t *default_sequence = NULL;

static void init_sequence(void)
{
    static seq_t seq[84];
    if (default_sequence != NULL)
        return;   /* already initialized */

    seq_t *p = seq;
    for (int i = 0; i < 9; ++i)
        for (int j = i + 1; j < 9; ++j)
            for (int k = j + 1; k < 9; ++k)
            {
                long long v =
                    (long long)xs[i] * ((long long)ys[j] - ys[k]) +
                    (long long)xs[j] * ((long long)ys[k] - ys[i]) +
                    (long long)xs[k] * ((long long)ys[i] - ys[j]);
                if (v < 0) v = -v;
                *p++ = (seq_t)v;
            }

    /* Sort the 84 distinct doubled areas. */
    qsort(seq, 84, sizeof(seq_t), compare_seq_t);
    default_sequence = seq;
}

seq_t a(idx_t n)
{
    if (n == 0)          return 0;   /* undefined for n = 0 */
    if (default_sequence == NULL) init_sequence();
    if (n > 84)          return 0;   /* sequence defined only for 1..84 */
    return default_sequence[n - 1];
}
