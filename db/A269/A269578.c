/*
 * A269578  Number of length-n 0..2 arrays with no repeated value differing
 *          from the previous repeated value by more than one.
 * https://oeis.org/A269578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) - 2*a(n-2) - 4*a(n-3) + a(n-4)
   for n ≥ 5, with a(1)=3, a(2)=9, a(3)=27, a(4)=79
   (OEIS A269578) */

#include <stdlib.h>   /* for malloc, realloc, free */

seq_t a(idx_t n)
{
    /* static state ----------------------------------------------------------- */
    static seq_t *buf = NULL;       /* array of computed values (index 0 unused) */
    static idx_t capacity = 0;      /* size of allocated buffer (in elements)   */
    static idx_t last     = 0;      /* highest index that has been computed     */

    /* invalid index ----------------------------------------------------------- */
    if (n < 1)
        return (seq_t)0;

    /* ensure buffer big enough ------------------------------------------------- */
    if (n + 1 > capacity) { /* need indices 1..n, so size n+1 */
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp)          /* allocation failed */
            return (seq_t)0;
        buf = tmp;
        capacity = new_cap;
    }

    /* initialise base values if not yet done ---------------------------------- */
    if (last < 4) {
        /* set first four terms */
        if (capacity < 5) { /* guarantee indices 1..4 exist */
            size_t need = 5;
            seq_t *tmp = (seq_t *)realloc(buf, need * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            buf = tmp;
            capacity = need;
        }
        buf[1] = (seq_t)3;
        buf[2] = (seq_t)9;
        buf[3] = (seq_t)27;
        buf[4] = (seq_t)79;
        last = 4;
    }

    /* compute further terms ---------------------------------------------------- */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t t1 = buf[i - 1] * (seq_t)4 + buf[i - 4];
        seq_t t2 = buf[i - 2] * (seq_t)2 + buf[i - 3] * (seq_t)4;
        buf[i] = t1 - t2;
    }
    last = n;

    return buf[n];
}
