/*
 * A252959  Number of (n+2)X(6+2) 0..4 arrays with every consecutive three
 *          elements in every row and diagonal having exactly two distinct
 *          values, and in every column and antidiagonal not having exactly
 *          two distinct values, and new values 0 upwards introduced in row
 *          major order.
 * https://oeis.org/A252959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the linear recurrence a(n) = 5*a(n-1) - 6*a(n-2) -
 *            2*a(n-3) - 5*a(n-4) + 15*a(n-5) + 53*a(n-6) -145*a(n-7) +
 *            120*a(n-8) - 44*a(n-9) + 25*a(n-10) - 21*a(n-11) + 6*a(n-12)
 *            with the initial values a(1)…a(15) as listed in the OEIS entry.
 *            The function memoises computed values in a static...
 * Range:     u8 none, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* memoisation array, allocated on first use */
static size_t cache_size = 0; /* number of terms currently stored */

/* signed recurrence coefficients, corresponding to the 12 terms of the
   recurrence above.  The array is indexed from 0 to 11, where coeff[0]
   applies to a(n-1), coeff[1] to a(n-2), …, coeff[11] to a(n-12). */
static const int coeff[12] = {
    5,  -6,  -2,  -5,  15, 53, -145, 120, -44,  25, -21,  6
};

/* first 15 terms of the sequence (a(1) to a(15)).  They are
   supplied by the OEIS entry and are used to initialise the cache. */
static const seq_t init15[15] = {
    (seq_t)3465, (seq_t)651, (seq_t)985, (seq_t)1551,  (seq_t)2822,
    (seq_t)5100, (seq_t)8767, (seq_t)16909, (seq_t)32096, (seq_t)57282,
    (seq_t)113321, (seq_t)218547, (seq_t)401170, (seq_t)797512, (seq_t)1556859
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n<1 */

    /* initialise the cache with the first 15 known terms on first call */
    if (cache == NULL) {
        cache_size = 15;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) exit(1); /* allocation failure */

        for (size_t i = 0; i < cache_size; ++i)
            cache[i] = init15[i];
    }

    /* extend the cache if the requested index is beyond the current size */
    if ((size_t)n > cache_size) {
        size_t new_size = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) exit(1);          /* allocation failure */
        cache = tmp;

        for (size_t i = cache_size; i < new_size; ++i) {
            /* compute a(i+1) : cache index i corresponds to a(i+1) */
            seq_t pos = 0;   /* sum of positive contributions */
            seq_t neg = 0;   /* sum of absolute values of negative contributions */

            /* i corresponds to n-1, so cache[i-1-j] refers to a(n-j-1) */
            for (int j = 0; j < 12; ++j) {
                int c = coeff[j];
                seq_t term = (seq_t)(c > 0 ? c : -c) * cache[i - 1 - j];
                if (c > 0)
                    pos += term;
                else
                    neg += term;
            }
            /* final result: pos - neg (pos > neg is guaranteed by the sequence) */
            cache[i] = pos - neg;
        }
        cache_size = new_size;
    }

    return cache[n - 1];
}
