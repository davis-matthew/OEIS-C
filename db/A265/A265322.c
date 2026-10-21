/*
 * A265322  Number of OFF (white) cells in the n-th iteration of the "Rule
 *          110" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A265322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 179, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation:  a(0) = 0;  a(n) = number of OFF (white) cells after n steps of Rule 110
   starting from a single ON (black) cell.  The function simulates the elementary
   cellular automaton and counts the zeros in each generation.  */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Rule 110 rule table: index = (left<<2)|(mid<<1)|right   (0≤idx≤7)  */
static const uint8_t rule110_table[8] = {
    0, /* 000 */
    1, /* 001 */
    1, /* 010 */
    1, /* 011 */
    0, /* 100 */
    1, /* 101 */
    1, /* 110 */
    0  /* 111 */
};

static seq_t *a_cache = NULL;       /* cache of a(n) values          */
static idx_t  a_cache_len = 0;      /* number of entries in a_cache  */

static uint8_t *row_buf = NULL;     /* buffer holding current row    */
static idx_t    row_len  = 0;       /* width of current row          */

/* Compute a(n) – number of OFF cells after n steps. */
seq_t a(idx_t n)
{
    /*----- initialise for first call (n = 0) ----------------------------------*/
    if (a_cache_len == 0) {
        a_cache_len = 1;
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        a_cache[0] = 0;                 /* a(0) = 0 */

        row_len = 1;                    /* width = 1 (2*0+1)             */
        row_buf = (uint8_t *)malloc(row_len);
        row_buf[0] = 1;                  /* initial single ON cell        */
    }

    /*----- extend cache up to requested n -----------------------------------*/
    if (n >= a_cache_len) {
        for (idx_t step = a_cache_len; step <= n; ++step) {
            idx_t new_len = 2 * step + 1;
            uint8_t *new_row = (uint8_t *)malloc(new_len);
            seq_t   ones = 0;

            /* Use correct alignment: new[j] depends on old[j-2], old[j-1], old[j] */
            for (idx_t j = 0; j < new_len; ++j) {
                uint8_t left  = 0, mid = 0, right = 0;

                if (j >= 2 && (j - 2) < row_len)   left  = row_buf[j - 2];
                if (j >= 1 && (j - 1) < row_len)   mid   = row_buf[j - 1];
                if (j < row_len)                   right = row_buf[j];

                uint8_t idx = (left << 2) | (mid << 1) | right;
                uint8_t out = rule110_table[idx];
                new_row[j] = out;
                if (out) ++ones;
            }

            /* a(step) = number of OFF cells = new_len - number of ON cells */
            seq_t a_val = (seq_t)new_len - ones;

            /* enlarge cache and store value */
            a_cache = (seq_t *)realloc(a_cache, (step + 1) * sizeof(seq_t));
            a_cache[step] = a_val;
            a_cache_len = step + 1;

            /* replace old row */
            free(row_buf);
            row_buf = new_row;
            row_len = new_len;
        }
    }

    return a_cache[n];
}
