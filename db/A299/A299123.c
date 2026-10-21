/*
 * A299123  Number of n X 3 0..1 arrays with every element equal to 0, 1, 3,
 *          4, 5 or 7 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A299123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) +4*a(n-2) +16*a(n-3) -13*a(n-4) -35*a(n-5)
 *            -62*a(n-6) * +28*a(n-7) +65*a(n-8) +36*a(n-9) +61*a(n-10)
 *            +59*a(n-11) * -34*a(n-12) -64*a(n-13) -36*a(n-14) -40*a(n-15)
 *            +3*a(n-16) * +18*a(n-17) +2*a(n-18) for n > 19 * Initial values
 *            (1‑based): 3, 4, 9, 23, 31, 184, 427, 1115, 4128, 11024,...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper to grow an array of seq_t */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*ptr, new_cap * elem_size);
        *ptr = tmp;
        *cap = new_cap;
    }
}

static const int coeff[18] = {
    1,  4,  16, -13, -35, -62, 28, 65, 36, 61, 59,
   -34, -64, -36, -40,   3, 18,  2
};

static const seq_t init_vals[19] = {
    3,          /* a(1) */
    4,          /* a(2) */
    9,          /* a(3) */
    23,         /* a(4) */
    31,         /* a(5) */
    184,        /* a(6) */
    427,        /* a(7) */
    1115,       /* a(8) */
    4128,       /* a(9) */
    11024,      /* a(10) */
    32827,      /* a(11) */
    104959,     /* a(12) */
    302093,     /* a(13) */
    917104,     /* a(14) */
    2804565,    /* a(15) */
    8324555,    /* a(16) */
    25231517,   /* a(17) */
    76267810,   /* a(18) */
    228857830   /* a(19) */
};

static seq_t *cache = NULL;      /* cache[0] unused, indices start at 1 */
static size_t cache_cap = 0;
static idx_t cached_upto = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n<1, return 0 for safety */

    /* Ensure cache has room for index n */
    ensure_capacity((size_t)n + 1, &cache_cap, (void **)&cache, sizeof(seq_t));

    /* Compute missing values up to n */
    for (idx_t i = cached_upto + 1; i <= n; ++i) {
        if (i <= 19) {               /* initial values */
            cache[i] = init_vals[i - 1];
        } else {                      /* recurrence */
            seq_t pos = 0;            /* sum of positive contributions */
            seq_t neg = 0;            /* sum of negative contributions (as positive) */
            for (int k = 1; k <= 18; ++k) {
                seq_t term = cache[i - k];
                int c = coeff[k - 1];
                if (c > 0) {
                    pos += term * (seq_t)c;
                } else if (c < 0) {
                    neg += term * (seq_t)(-c);
                }
            }
            cache[i] = pos - neg;     /* pos >= neg for every n */
        }
    }
    cached_upto = n;
    return cache[n];
}
