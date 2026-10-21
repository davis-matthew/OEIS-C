/*
 * A285794  a(1)=1, a(2)=2, a(3)=3, a(n) = 3*a(n-3)+2*a(n-2)+a(n-1).
 * https://oeis.org/A285794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2, a(3)=3, a(n)=3*a(n-3)+2*a(n-2)+a(n-1) for n≥4
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 52, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf   = NULL;   /* cache array */
    static size_t buf_sz = 0;     /* number of terms cached */
    static size_t buf_cap = 0;    /* allocated capacity */

    /* n is ≥1 by definition; convert once for array indices */
    size_t needed = (size_t)n;
    size_t i;

    /* ensure we have space for at least the first three terms */
    if (buf_cap < 3) {
        size_t new_cap = 3;
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* failed allocation */
        buf = tmp;
        buf_cap = new_cap;
        if (buf_sz == 0) {
            buf[0] = (seq_t)1;   /* a(1) */
            buf[1] = (seq_t)2;   /* a(2) */
            buf[2] = (seq_t)3;   /* a(3) */
            buf_sz = 3;
        }
    }

    /* grow capacity if needed (doubling strategy) */
    if (needed > buf_cap) {
        size_t new_cap = buf_cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        buf = tmp;
        buf_cap = new_cap;
    }

    /* compute missing terms up to needed */
    if (buf_sz < needed) {
        for (i = buf_sz; i < needed; ++i) {
            buf[i] = buf[i-3] * (seq_t)3
                   + buf[i-2] * (seq_t)2
                   + buf[i-1];
        }
        buf_sz = needed;
    }

    return buf[needed - 1];
}
