/*
 * A251230  Number of (n+1) X (2+1) 0..3 arrays with every 2 X 2 subblock
 *          summing to 6 and no 2 X 2 subblock having exactly two nonzero
 *          entries.
 * https://oeis.org/A251230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 17, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 11*a(n-1) - 42*a(n-2) + 49*a(n-3) + 69*a(n-4)
          - 184*a(n-5) + 18*a(n-6) + 160*a(n-7)
          - 32*a(n-8)   - 48*a(n-9)
   Initial values:
   a(1)=110, a(2)=270, a(3)=694, a(4)=1870, a(5)=5194,
   a(6)=14786, a(7)=42850, a(8)=125934, a(9)=374166
*/
#include <stdlib.h>

static const seq_t init_tbl[9] = {
    110, 270, 694, 1870, 5194, 14786, 42850, 125934, 374166
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for offset 1 */

    /* static state for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;        /* allocated capacity */
    static size_t count = 0;        /* number of terms stored */

    /* initialise cache on first call */
    if (cache == NULL)
    {
        /* start with a capacity that can hold at least 9 terms */
        cap   = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;    /* allocation failure */

        /* copy initial terms safely – use assignment, not memcpy */
        for (size_t i = 0; i < 9; ++i)
            cache[i] = init_tbl[i];

        count = 9;
    }

    size_t idx = (size_t)n - 1;     /* zero‑based index for a(n) */

    /* compute missing terms up to idx */
    while (count <= idx)
    {
        /* ensure enough space */
        if (count >= cap)
        {
            size_t new_cap = cap * 2;
            if (new_cap < count + 1) new_cap = count + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0;   /* allocation failure */
            cache = tmp;
            cap   = new_cap;
        }

        seq_t term =   (seq_t)11   * cache[count-1]
                     - (seq_t)42   * cache[count-2]
                     + (seq_t)49   * cache[count-3]
                     + (seq_t)69   * cache[count-4]
                     - (seq_t)184  * cache[count-5]
                     + (seq_t)18   * cache[count-6]
                     + (seq_t)160  * cache[count-7]
                     - (seq_t)32   * cache[count-8]
                     - (seq_t)48   * cache[count-9];

        cache[count++] = term;
    }

    return cache[idx];
}
