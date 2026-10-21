/*
 * A265687  Numbers k such that the concave polygon formed by the trajectory
 *          of k in the "3x+1" problem and the straight line between the
 *          coordinate points (0,k) and (r,1) where r is the number of
 *          iterations needed to reach 1 is not a self-intersecting polygon.
 * https://oeis.org/A265687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A265687: numbers k for which the Collatz trajectory of k lies strictly
   above the straight line joining (0,k) and (r,1), where r is the
   number of steps needed to reach 1.  The function a(n) returns the
   n‑th such number.  Computation is performed by brute‑force search
   with memoisation of the already found values. */

#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Static state used by a()                                            */
/* --------------------------------------------------------------------- */
static seq_t    *seq    = NULL;   /* array of found terms      */
static size_t    seq_len  = 0;    /* number of stored terms    */
static size_t    seq_cap  = 0;    /* current capacity of seq   */

static seq_t    *path    = NULL;   /* temporary buffer for a path */
static size_t    path_cap = 0;     /* current capacity of path    */

static idx_t     next      = 3;     /* next candidate to test      */

/* --------------------------------------------------------------------- */
/*  Helpers to grow the dynamic buffers                                */
/* --------------------------------------------------------------------- */
static void ensure_seq_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    seq_cap = seq_cap ? seq_cap * 2 : 8;
    seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
}

static void ensure_path_cap(size_t needed)
{
    if (path_cap >= needed) return;
    path_cap = path_cap ? path_cap * 2 : 16;
    path = (seq_t *)realloc(path, path_cap * sizeof(seq_t));
}

/* --------------------------------------------------------------------- */
/*  Main function                                                        */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Repeat until we have at least n terms recorded */
    while (seq_len < (size_t)n) {
        /* Generate the Collatz trajectory of candidate 'next'   */
        seq_t cand = (seq_t)next;
        size_t len = 0;
        seq_t x = cand;

        do {
            ensure_path_cap(len + 1);
            path[len++] = x;
            if (x == (seq_t)1) break;

            /* Collatz step */
            if ((x & (seq_t)1) == 0)
                x = x >> 1;
            else
                x = x * (seq_t)3 + (seq_t)1;
        } while (1);

        size_t r = len - 1;          /* number of iterations to reach 1 */

        /* Verify that the trajectory stays above the line f(i) = k - (k-1)*i/r  */
        bool ok = true;
        if (r >= 1) {
            seq_t lhs = cand * (seq_t)r;
            for (size_t i = 1; i < r; ++i) {
                seq_t val = path[i];
                /* lhs < rhs where rhs = val*r + (cand-1)*i          */
                seq_t rhs = val * (seq_t)r + ((cand - (seq_t)1) * (seq_t)i);
                if (!(lhs < rhs)) {
                    ok = false;
                    break;
                }
            }
        }

        if (ok) {
            ensure_seq_cap(seq_len + 1);
            seq[seq_len++] = cand;
        }

        ++next;
    }

    return seq[(size_t)n - 1];
}
