/*
 * A383840  Expansion of 1/((1-x) * (1-4*x) * (1-9*x) * (1-16*x) * (1-25*x)).
 * https://oeis.org/A383840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A383840: Expansion of 1/((1-x)*(1-4*x)*(1-9*x)*(1-16*x)*(1-25*x))
 * Recurrence: a(0)=1, a(1)=55, a(2)=2002, a(3)=61490, a(4)=1733303
 *              a(n) = 55*a(n-1) - 1023*a(n-2) + 7645*a(n-3) - 21076*a(n-4) + 14400*a(n-5)
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Small initial values */
    static const seq_t init[5] = {1, 55, 2002, 61490, 1733303};

    /* Static cache of computed values */
    static seq_t *seq = NULL;
    static size_t cap = 0;      /* allocated size of seq buffer */
    static idx_t last = 4;      /* index of last computed element (>=4 if seq allocated) */

    /* Handle the first five terms directly */
    if (n < 5)
        return init[n];

    /* Allocate initial buffer if necessary */
    if (!seq) {
        size_t needed = (size_t)n + 1;
        if (needed < 5) needed = 5;
        seq = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!seq) { /* Allocation failed – return 0 as sentinel */
            return (seq_t)0;
        }
        cap = needed;
        /* Initialize first five terms */
        for (idx_t i = 0; i < 5; ++i)
            seq[i] = init[i];
        last = 4;
    }

    /* Ensure the buffer is large enough */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 5;
        while ((size_t)(n + 1) > new_cap) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) { /* Reallocation failed – return 0 as sentinel */
            return (seq_t)0;
        }
        seq = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            seq[i] = 55*seq[i-1]
                     - 1023*seq[i-2]
                     + 7645*seq[i-3]
                     - 21076*seq[i-4]
                     + 14400*seq[i-5];
        }
        last = n;
    }
    return seq[n];
}
