/*
 * A267913  Number of 2 X n 0..2 arrays with every repeated value in every row
 *          unequal to the previous repeated value, and in every column equal
 *          to the previous repeated value, and new values introduced in
 *          row-major sequential order.
 * https://oeis.org/A267913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A267913
 * Recurrence: a(n) = 11*a(n-1) - 2*a(n-2) - 288*a(n-3) + 544*a(n-4)
 *                +1792*a(n-5) - 4096*a(n-6)   for n > 8
 * Initial values:
 * a(1)=  2   a(2)= 14   a(3)= 96   a(4)= 726
 * a(5)= 5046  a(6)=35574  a(7)=242406  a(8)=1653750
 */

#include <stddef.h>
#include <stdlib.h>

/* helper to grow the internal buffer for both C and C++ */
static void ensure_capacity(idx_t needed, seq_t **buf, size_t *alloc, size_t *used)
{
    if (*alloc >= (size_t)needed) return;

    size_t newsize = *alloc ? (*alloc << 1) : 16;
    while (newsize < (size_t)needed) newsize <<= 1;

#if defined(__cplusplus)
    seq_t *newbuf = new seq_t[newsize];
    for (size_t i = 0; i < *used; ++i) newbuf[i] = (*buf)[i];
    delete[] *buf;
#else
    seq_t *newbuf = (seq_t *)realloc(*buf, newsize * sizeof(seq_t));
#endif

    *buf = newbuf;
    *alloc = newsize;
}

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *buf   = NULL;   /* array of computed terms          */
    static idx_t used   = 0;      /* number of terms stored           */
    static size_t alloc = 0;      /* allocated size of buf            */

    /* initialize with the first eight terms if not yet done */
    if (used == 0) {
        used   = 8;
        alloc  = 8;
#if defined(__cplusplus)
        buf = new seq_t[alloc];
#else
        buf = (seq_t *)malloc(alloc * sizeof(seq_t));
#endif
        buf[0] = (seq_t)2ULL;
        buf[1] = (seq_t)14ULL;
        buf[2] = (seq_t)96ULL;
        buf[3] = (seq_t)726ULL;
        buf[4] = (seq_t)5046ULL;
        buf[5] = (seq_t)35574ULL;
        buf[6] = (seq_t)242406ULL;
        buf[7] = (seq_t)1653750ULL;
    }

    /* if requested term is already known, return it */
    if (n <= used) return buf[n-1];

    /* make sure buffer large enough */
    ensure_capacity(n, &buf, &alloc, &used);

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = used; i < n; ++i) {
        /* indices: buf[i-1] = a(i), ..., buf[i-6] = a(i-5) */
        seq_t a1 = buf[i-1];
        seq_t a2 = buf[i-2];
        seq_t a3 = buf[i-3];
        seq_t a4 = buf[i-4];
        seq_t a5 = buf[i-5];
        seq_t a6 = buf[i-6];

        seq_t pos = a1 * (seq_t)11;
        pos += a4 * (seq_t)544;
        pos += a5 * (seq_t)1792;

        seq_t neg = a2 * (seq_t)2;
        neg += a3 * (seq_t)288;
        neg += a6 * (seq_t)4096;

        buf[i] = pos - neg;
    }
    used = n;
    return buf[n-1];
}
