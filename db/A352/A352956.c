/*
 * A352956  For n>1, a(n) = a(n - a(n-1)) + a(n-2) starting with a(0) = a(1) =
 *          1.
 * https://oeis.org/A352956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = 1; for n ≥ 2, if n is even: a(n) = a(n-2) +
 *            a(n/2); if n is odd : a(n) = ((n-1)/2) + 1 (Equivalent to
 *            a(2*k+1)=k+1 and a(2*k+2)=a(2*k)+a(k+1))
 * Range:     u8 n <= 41, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;
    static size_t cap   = 0;
    static size_t max_i = 0;   /* highest index that is already computed */

    /* first‑time initialization */
    if (!buf) {
        cap = 2;                   /* at least store indices 0 and 1 */
        buf = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!buf) return (seq_t)0; /* OOM fallback */
        buf[0] = (seq_t)1;
        buf[1] = (seq_t)1;
        max_i  = 1;
    }

    /* if requested index already known, return it */
    if ((size_t)n <= max_i)
        return buf[(size_t)n];

    /* enlarge the buffer if necessary */
    size_t needed = (size_t)n + 1;          /* need indices up to n */
    if (needed > cap) {
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 2;
        while (new_cap < needed)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* OOM fallback */
        buf = tmp;
        cap = new_cap;
    }

    /* compute values from max_i+1 up to n */
    for (size_t i = max_i + 1; i <= (size_t)n; ++i) {
        if (i & 1) {                                 /* odd */
            buf[i] = (seq_t)(((i - 1) / 2) + 1);
        } else { /* even and i >= 2 */
            buf[i] = buf[i - 2] + buf[i / 2];
        }
    }

    max_i = (size_t)n;
    return buf[(size_t)n];
}
