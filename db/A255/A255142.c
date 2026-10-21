/*
 * A255142  Number of (n+2)X(1+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum 0 and no antidiagonal sum 0 and no row sum 1 and no column sum
 *          1.
 * https://oeis.org/A255142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 19, u64 n <= 41, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A255142
   Recurrence (OEIS formula):
   a(n) = 2*a(n-1) + 8*a(n-3)
          - 2*a(n-4) - 6*a(n-5) - 16*a(n-6)
          - 13*a(n-7) + 6*a(n-8) + 14*a(n-9)
          + 12*a(n-10) - 4*a(n-12) - 4*a(n-13)
   with initial values (for n ≥ 1):
   a(1) = 47
   a(2) = 115
   a(3) = 302
   a(4) = 784
   a(5) = 2086
   a(6) = 5661
   a(7) = 15406
   a(8) = 41999
   a(9) = 114801
   a(10) = 314146
   a(11) = 859864
   a(12) = 2354180
   a(13) = 6446675
*/

#include <stdlib.h>

/* forward declaration of the sequence function */
seq_t a(idx_t n);

/* static cache for memoisation */
static seq_t *cache = NULL;   /* 1‑based array of computed terms */
static idx_t cached_size = 0; /* highest index that has been computed */

/* hard‑coded initial values for n = 1 … 13, with a dummy entry at 0 */
static const seq_t base_vals[14] = {
    (seq_t)0,      /* unused 0‑th element */
    (seq_t)47,     /* a(1) */
    (seq_t)115,    /* a(2) */
    (seq_t)302,    /* a(3) */
    (seq_t)784,    /* a(4) */
    (seq_t)2086,   /* a(5) */
    (seq_t)5661,   /* a(6) */
    (seq_t)15406,  /* a(7) */
    (seq_t)41999,  /* a(8) */
    (seq_t)114801, /* a(9) */
    (seq_t)314146, /* a(10) */
    (seq_t)859864, /* a(11) */
    (seq_t)2354180,/* a(12) */
    (seq_t)6446675 /* a(13) */
};

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;        /* undefined for n < 1 */

    /* initialise cache on the first call */
    if (cache == NULL) {
        /* allocate space for indices 0 … 13 */
        cache = (seq_t *)malloc(14 * sizeof(seq_t));
        if (cache == NULL)
            return 0;   /* allocation failure */

        /* store the initial values */
        for (idx_t i = 1; i <= 13; ++i)
            cache[i] = base_vals[i];
        cache[0] = 0;
        cached_size = 13;
    }

    /* return immediately if we already know a(n) */
    if (n <= cached_size)
        return cache[n];

    /* enlarge cache to hold up to index n */
    idx_t old_size = cached_size;
    if (n > old_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL)
            return 0;   /* allocation failure */
        cache = tmp;

        /* compute missing terms using the recurrence */
        for (idx_t i = old_size + 1; i <= n; ++i) {
            seq_t val = 2 * cache[i - 1];          // +2*a(i-1)
            val += 8 * cache[i - 3];               // +8*a(i-3)
            val -= 2 * cache[i - 4];               // -2*a(i-4)
            val -= 6 * cache[i - 5];               // -6*a(i-5)
            val -= 16 * cache[i - 6];              // -16*a(i-6)
            val -= 13 * cache[i - 7];              // -13*a(i-7)
            val += 6 * cache[i - 8];               // +6*a(i-8)
            val += 14 * cache[i - 9];              // +14*a(i-9)
            val += 12 * cache[i - 10];             // +12*a(i-10)
            val -= 4 * cache[i - 12];              // -4*a(i-12)
            val -= 4 * cache[i - 13];              // -4*a(i-13)
            cache[i] = val;
        }

        cached_size = n;
    }

    return cache[n];
}
