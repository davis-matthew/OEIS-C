/*
 * A355460  Imaginary part of the Heighway/Harter dragon curve points which
 *          are on the imaginary axis.
 * https://oeis.org/A355460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 49, i16 n <= 49, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A355460 – Imaginary part of the Heighway/Harter dragon curve points
 * that lie on the imaginary (x = 0) axis.
 * The function is memoised: values are generated in order until the
 * requested index has been produced, recording the y‑coordinate each
 * time the walk is on the axis.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* seq_t and idx_t are provided by the build system.  No re‑definition
 * is necessary – they are already typedef'd in an included header.  */

static seq_t   *axis      = NULL;   /* stored y‑coordinates of axis points */
static size_t   axis_len  = 0;       /* number of terms currently cached   */
static size_t   axis_cap  = 0;       /* allocated capacity of 'axis'       */

static int      dir       = 0;       /* 0=east,1=north,2=west,3=south      */
static seq_t    cur_x     = 0, cur_y = 0; /* current position            */
static idx_t    step_num  = 0;       /* number of steps performed          */

static void grow_axis(size_t needed)
{
    if (needed <= axis_cap)
        return;
    size_t new_cap = axis_cap ? axis_cap << 1 : 32;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(axis, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – abort the program (behaviour undefined
           for the OEIS test harness but acceptable for simple CP builds).*/
        abort();
    }
    axis = tmp;
    axis_cap = new_cap;
}

/* Ensure that at least up to index 'target' has been produced.  */
static void fill_to(idx_t target)
{
    /* First initialise on the very first call. */
    if (axis_len == 0) {
        grow_axis(32);
        axis[0] = (seq_t)0;  /* a(0) = 0 (starting point) */
        axis_len = 1;

        cur_x = 0;
        cur_y = 0;
        dir   = 0;               /* heading east initially */
        step_num = 0;
    }

    while (axis_len <= target) {
        /* Advance one step. */
        step_num++;         /* step index (1‑based) */

        switch (dir) {
            case 0: cur_x += 1; break;   /* east   */
            case 1: cur_y += 1; break;   /* north  */
            case 2: cur_x -= 1; break;   /* west   */
            case 3: cur_y -= 1; break;   /* south  */
        }

        /* Record the y‑coordinate if we are on the imaginary axis. */
        if (cur_x == 0) {
            grow_axis(axis_len + 1);
            axis[axis_len++] = cur_y;
        }

        /* Determine the turn for the next step using the standard
           dragon‑curve rule: left if ((t & -t) << 1) & t == 0,
           otherwise right (where t is the current step number). */
        idx_t low  = step_num & (~(step_num - 1));   /* lowest set bit of t */
        idx_t test = low << 1;
        if ((test & step_num) == 0)               /* left turn   */
            dir = (dir + 1) & 3;
        else                                      /* right turn  */
            dir = (dir + 3) & 3;
    }
}

seq_t a(idx_t n)
{
    fill_to(n);
    return axis[n];
}
