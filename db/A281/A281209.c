/*
 * A281209  Number of 5 X n 0..1 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors, with the exception
 *          of exactly one element, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A281209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 34, u64 n <= 79, u128 n <= 170,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (empirical): a(n) = 4*a(n-1) - 4*a(n-2) - 2*a(n-3) + 4*a(n-4) - a(n-6) for n>11.
 * Initial values (for n = 1..11):
 *  a(1)=0, a(2)=130, a(3)=448, a(4)=676, a(5)=1141,
 *  a(6)=1906, a(7)=3137, a(8)=5160, a(9)=8510,
 *  a(10)=14084, a(11)=23379. */

#include <stdlib.h>

static seq_t *seq_cache = NULL;      /* array holding computed values */
static idx_t seq_max   = 0;          /* highest n computed (1‑based) */
static size_t seq_cap  = 0;          /* allocated capacity of seq_cache */

/* Compute a(n) for any n >= 1.  Calls may occur in any order. */
seq_t a(idx_t n)
{
    /* Ensure cache is initialized with the first 11 terms. */
    if (seq_cache == NULL) {
        /* Allocate enough space for the requested n, but at least 13
           entries (index 0 unused). */
        seq_cap = (n > 12 ? n + 1 : 13);
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        /* Initialize the cache with the 11 known values. */
        seq_cache[0] = 0;                /* unused */
        seq_cache[1] = 0;
        seq_cache[2] = 130;
        seq_cache[3] = 448;
        seq_cache[4] = 676;
        seq_cache[5] = 1141;
        seq_cache[6] = 1906;
        seq_cache[7] = 3137;
        seq_cache[8] = 5160;
        seq_cache[9] = 8510;
        seq_cache[10] = 14084;
        seq_cache[11] = 23379;
        seq_max = 11;
    }
    /* Resize the cache if the requested n exceeds the current capacity. */
    else if ((idx_t)seq_cap <= n) {
        seq_cap = (size_t)(n + 1);
        seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
    }

    /* If we already have the value, return it immediately. */
    if (n <= seq_max)
        return seq_cache[n];

    /* Compute the remaining terms using the recurrence. */
    for (idx_t i = seq_max + 1; i <= n; ++i) {
        /* 4*a(i-1) - 4*a(i-2) - 2*a(i-3) + 4*a(i-4) - a(i-6) */
        seq_t val = (seq_cache[i-1] << 2);          /* 4*a(i-1) */
        val -= (seq_cache[i-2] << 2);              /* -4*a(i-2) */
        val -= (seq_cache[i-3] << 1);              /* -2*a(i-3) */
        val += (seq_cache[i-4] << 2);              /* +4*a(i-4) */
        if (i >= 6)
            val -= seq_cache[i-6];                 /* -a(i-6) */
        seq_cache[i] = val;
    }
    seq_max = n;
    return seq_cache[n];
}
