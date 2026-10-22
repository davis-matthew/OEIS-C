/*
 * A398355  Number of previously visited grid points visible after taking n
 *          steps along the square spiral.
 * https://oeis.org/A398355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   abs64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A398355: number of previously visited grid points visible after n steps
 * (square spiral).  Computed by brute‑force:
 *  a(n) = |{ i < n : gcd(|x_n-x_i|, |y_n-y_i|) == 1 }|
 *  where (x_k,y_k) are the lattice coordinates after k steps of the
 *  standard square spiral starting at (0,0).
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/abs64.h"

static inline int64_t gcd64(int64_t a, int64_t b)
{
    a = abs64(a);
    b = abs64(b);
    while (b != 0) {
        int64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0)
        return (seq_t)0;

    /* static state shared between calls */
    static idx_t cached_max = 0;                 /* largest n computed so far */
    static int64_t *x = NULL, *y = NULL;        /* coordinates */
    static seq_t *cnt = NULL;                   /* a(k) values */
    static size_t capacity = 0;                 /* allocated array size */

    /* spiral state */
    static int dir = 0;          /* 0:right, 1:up, 2:left, 3:down */
    static int step_len = 1;     /* current segment length */
    static int seg_done = 0;     /* number of completed segments */
    static int seg_rem = 1;      /* steps remaining in current segment */
    static int64_t cur_x = 0, cur_y = 0; /* current position */

    /* ensure capacity for indices up to n */
    if (capacity <= (size_t)n) {
        size_t new_cap = (capacity == 0) ? 1 : capacity * 2;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        int64_t *tmp_x = (int64_t *)realloc(x, new_cap * sizeof(int64_t));
        int64_t *tmp_y = (int64_t *)realloc(y, new_cap * sizeof(int64_t));
        seq_t  *tmp_c = (seq_t *)realloc(cnt, new_cap * sizeof(seq_t));
        if (!tmp_x || !tmp_y || !tmp_c) { /* allocation failure */
            return (seq_t)0;
        }
        x = tmp_x;
        y = tmp_y;
        cnt = tmp_c;
        capacity = new_cap;
    }

    /* initialise data for the very first call */
    if (cached_max == 0) {
        x[0] = 0;
        y[0] = 0;
        cnt[0] = 0;
        /* initial spiral state already set to match start at origin */
    }

    /* compute new points up to n */
    for (idx_t k = cached_max + 1; k <= n; ++k) {
        /* step one unit in current direction */
        switch (dir) {
            case 0: ++cur_x; break; /* right   */
            case 1: ++cur_y; break; /* up      */
            case 2: --cur_x; break; /* left    */
            case 3: --cur_y; break; /* down    */
        }
        x[k] = cur_x;
        y[k] = cur_y;

        /* update segment counters */
        --seg_rem;
        if (seg_rem == 0) {
            dir = (dir + 1) & 3;
            ++seg_done;
            if ((seg_done & 1) == 0)
                ++step_len;
            seg_rem = step_len;
        }

        /* count visible previous points */
        unsigned int visible = 0;
        for (idx_t i = 0; i < k; ++i) {
            int64_t dx = x[i] - cur_x;
            int64_t dy = y[i] - cur_y;
            if (gcd64(dx, dy) == 1)
                ++visible;
        }
        cnt[k] = (seq_t)visible;
    }

    cached_max = n;
    return cnt[n];
}
