/*
 * A280598  Number of n X 2 0..1 arrays with no element equal to more than two
 *          of its horizontal, vertical and antidiagonal neighbors and with
 *          new values introduced in order 0 sequentially upwards.
 * https://oeis.org/A280598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1)= 2,  a(2)= 7,  a(3)=18,  a(4)=50,  a(5)=138,  a(6)=383
   a(n)=3*a(n-1)-a(n-2)+a(n-3)+a(n-4)-2*a(n-5)-a(n-6)   for n>=7   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial terms for the recurrence (index 1 through 6) */
    static const seq_t init[7] = {0, 2, 7, 18, 50, 138, 383};

    /* on first call (or when buffer needs to grow) we keep a cache
       of already-computed terms up to the largest n requested so far. */
    static seq_t *buf      = NULL;      /* cache array, indexed from 1 */
    static idx_t buf_size  = 0;          /* allocated size (in indices) */
    static idx_t max_comp  = 0;          /* largest index already computed */

    /* handle the tiny arguments immediately */
    if (n < 7) {
        return init[n];
    }

    /* allocate or grow the cache to accommodate the requested index */
    if (buf == NULL) {
        buf_size = n + 1;                /* guarantee space up to n */
        buf = (seq_t *)malloc(buf_size * sizeof(seq_t));
        if (!buf) abort();               /* a fatal failure */
        /* initialise the first six entries */
        for (idx_t i = 1; i <= 6; ++i) {
            buf[i] = init[i];
        }
    } else if (n + 1 > buf_size) {
        size_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(buf, new_size * sizeof(seq_t));
        if (!tmp) abort();
        buf = tmp;
        buf_size = new_size;
        /* initialise any new indices that are still in the initial range */
        for (idx_t i = max_comp + 1; i <= 6 && i < buf_size; ++i) {
            buf[i] = init[i];
        }
    }

    /* compute any terms that are still missing */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        if (i <= 6) {
            buf[i] = init[i];
        } else {
            buf[i] = 3 * buf[i - 1]
                   - buf[i - 2]
                   + buf[i - 3]
                   + buf[i - 4]
                   - 2 * buf[i - 5]
                   - buf[i - 6];
        }
    }
    max_comp = n;      /* remember how far we have computed */
    return buf[n];
}
