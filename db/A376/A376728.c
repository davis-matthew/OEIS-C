/*
 * A376728  Expansion of (1 + x^4 - x^5)/((1 + x^4 - x^5)^2 - 4*x^4).
 * https://oeis.org/A376728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-4) + 2*a(n-5) - a(n-8) + 2*a(n-9) - a(n-10);
 *            Initial values: a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=3, a(5)=1,
 *            a(6)=0, a(7)=0, a(8)=5, a(9)=10
 * Range:     u8 n <= 21, u16 n <= 39, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation; grows as needed. */
    static seq_t *cache = NULL;
    static idx_t cap = 0;      /* allocated capacity */
    static idx_t used = 0;     /* number of elements already computed */
    static const seq_t base[10] = {1,0,0,0,3,1,0,0,5,10};

    /* Ensure the array is large enough for index n. */
    if (n + 1 > cap) {
        cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure – return 0 */
        cache = tmp;
    }

    /* Compute missing values up to and including n. */
    if (used <= n) {
        for (idx_t i = used; i <= n; ++i) {
            if (i < 10) {
                cache[i] = base[i];          /* initial segment */
            } else {
                /* Compute via recurrence:
                   a(n) = 2*a(n-4) + 2*a(n-5) + 2*a(n-9) - a(n-8) - a(n-10) */
                seq_t pos = cache[i-4];
                pos += cache[i-4];            /* 2*a(n-4) */
                pos += cache[i-5];
                pos += cache[i-5];            /* 2*a(n-5) */
                pos += cache[i-9];
                pos += cache[i-9];            /* 2*a(n-9) */
                seq_t sub = cache[i-8];
                sub += cache[i-10];           /* a(n-8)+a(n-10) */
                cache[i] = pos - sub;         /* final value */
            }
        }
        used = n + 1;                       /* all up to n are ready */
    }

    return cache[n];
}
