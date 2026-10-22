/*
 * A335359  a(n) is the Y-coordinate of the n-th point of the Koch curve;
 *          sequence A335358 gives X-coordinates.
 * https://oeis.org/A335359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Y‑coordinate of the n‑th vertex of the Koch curve on a
 *            hexagonal lattice. The value is obtained by a depth‑first
 *            traversal of the 4‑fold subdivision, tracking only the current
 *            Y‑coordinate
 * Range:     u8 n <= 5156, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* state for the on‑demand generation of the sequence */
static seq_t *Y_seq = NULL;    /* array of Y‑coordinates */
static size_t Y_cap = 0;       /* allocated capacity of Y_seq */
static size_t Y_len = 0;       /* number of entries stored in Y_seq */
static seq_t cur_y;            /* current Y while drawing */
static int cur_orient = 0;     /* current orientation (0..5) */

/* offset of the four child segments relative to the parent orientation.
   Child 0: same direction
   Child 1: +60°
   Child 2: –60°
   Child 3: same direction again
*/
static const int child_offset[4] = {0, 1, -1, 0};

/* Recursive routine that draws a segment of the Koch curve at a given depth.
   A depth of 0 means a single unit step.  For each unit step the Y‑coordinate
   is updated according to the orientation and the new Y value is stored. */
static void draw(int depth)
{
    if (depth == 0)
    {
        /* update cur_y according to the current orientation */
        if (cur_orient == 1 || cur_orient == 2)    /* +60° */
            cur_y++;
        else if (cur_orient == 4 || cur_orient == 5) /* –60° */
            cur_y--;
        /* else: 0°, 180° – no vertical change */

        /* store the new Y value */
        Y_seq[Y_len++] = cur_y;
    }
    else
    {
        int parent_orient = cur_orient;
        for (int i = 0; i < 4; ++i)
        {
            int new_orient = parent_orient + child_offset[i];
            /* wrap into 0..5 */
            if (new_orient < 0)
                new_orient += 6;
            else if (new_orient >= 6)
                new_orient -= 6;

            cur_orient = new_orient;
            draw(depth - 1);
        }
        /* cur_orient left at the orientation of the last child,
           which is the correct orientation for the continuation. */
    }
}

/* Ensure that the sequence has been generated up to index n.
   This routine recomputes the entire array each time; the size is
   at most 4^depth+1 with depth chosen so that the array length exceeds n. */
static void ensure_generated_up_to(idx_t n)
{
    /* determine minimal depth such that 4^depth + 1 > n */
    idx_t pow4 = 1;
    int depth = 0;
    while (pow4 + 1 <= n)
    {
        pow4 *= 4;
        ++depth;
    }
    size_t needed = (size_t)pow4 + 1;          /* number of vertices needed */

    /* allocate / reallocate the array */
    Y_seq = (seq_t *)realloc(Y_seq, needed * sizeof(seq_t));
    Y_cap = needed;
    Y_len = 0;

    /* initialise traversal */
    cur_y = 0;
    cur_orient = 0;
    Y_seq[Y_len++] = cur_y;   /* vertex 0 */

    /* generate all vertices up to the required depth */
    draw(depth);
}

/* Public function required by the exercise.  It returns the Y‑coordinate
   of the n‑th vertex of the Koch curve. */
seq_t a(idx_t n)
{
    if (n >= Y_len)
        ensure_generated_up_to(n);   /* generate up to needed length */

    return Y_seq[n];
}
