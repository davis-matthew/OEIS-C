/*
 * A252964  Number of (3+2) X (n+2) 0..4 arrays with every consecutive three
 *          elements in every row and diagonal having exactly two distinct
 *          values, and in every column and antidiagonal not having exactly
 *          two distinct values, and new values 0 upwards introduced in row
 *          major order.
 * https://oeis.org/A252964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 14, u32 n <= 36, u64 n <= 77, u128 n <= 157,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A252964
    Empirical linear recurrence (n > 16):
      a(n) = a(n-1) + 6*a(n-2) - a(n-3)
           - 15*a(n-4) - 20*a(n-5) + 27*a(n-6)
           + 53*a(n-7) - 14*a(n-8) - 51*a(n-9)
           - 45*a(n-10) + 60*a(n-11) + 18*a(n-12)
           - 18*a(n-13)
     Initial values (1‑based):
       a(1)=297, a(2)=185, a(3)=259, a(4)=369, a(5)=605,
       a(6)=985, a(7)=1429, a(8)=2421, a(9)=3907, a(10)=6027,
       a(11)=9989, a(12)=16623, a(13)=25801, a(14)=43767,
       a(15)=71599, a(16)=116257
*/

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

/* 17 entries: index 0 unused (placeholder) */
static const seq_t init[17] = {
    0ULL,
    297ULL, 185ULL, 259ULL, 369ULL, 605ULL, 985ULL,
    1429ULL, 2421ULL, 3907ULL, 6027ULL, 9989ULL,
    16623ULL, 25801ULL, 43767ULL, 71599ULL, 116257ULL
};

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n < 1 */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(17 * sizeof(seq_t));
        if (cache == NULL)
            return 0; /* allocation failure – return 0 as sentinel */
        for (size_t i = 0; i < 17; ++i)
            cache[i] = init[i];
        cache_size = 17;
    }

    /* Extend cache if needed */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)
            return 0; /* on failure keep old cache, return 0 */
        cache = tmp;

        /* Compute missing terms */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            if (i <= 16) {
                cache[i] = init[i];
                continue;
            }

            /* sum of positive contributions */
            seq_t pos = cache[i-1];
            pos += 6 * cache[i-2];
            pos += 27 * cache[i-6];
            pos += 53 * cache[i-7];
            pos += 60 * cache[i-11];
            pos += 18 * cache[i-12];

            /* sum of negative contributions */
            seq_t neg = cache[i-3];
            neg += 15 * cache[i-4];
            neg += 20 * cache[i-5];
            neg += 14 * cache[i-8];
            neg += 51 * cache[i-9];
            neg += 45 * cache[i-10];
            neg += 18 * cache[i-13];

            cache[i] = pos - neg;   /* guaranteed non‑negative */
        }
        cache_size = new_size;
    }

    return cache[n];
}
