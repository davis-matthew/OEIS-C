/*
 * A358095  a(n) is the number of ways n can be reached in the algorithm
 *          explained in A358094 if the last operation is summation.
 * https://oeis.org/A358095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=0, a(3)=1, a(4)=2, a(5)=2 For n>5 use the piecewise
 *            recurrence a(6k) = a(2k-1)+a(3k-1) a(6k+1) = a(3k-1) a(6k+2) =
 *            a(2k)+a(3k) a(6k+3) = a(2k)+a(3k) a(6k+4) = a(3k+1) a(6k+5) =
 *            a(2k+1)+a(3k+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* static state */
    static seq_t *buf = NULL;
    static idx_t cap   = 0;   /* allocated capacity (number of elements)      */
    static idx_t size  = 0;   /* number of terms computed (>=5 after init)    */

    if (n == 0) return (seq_t)0;          /* safety, offsets start at 1           */

    /* initialise base terms a(1)..a(5) on first call */
    if (cap == 0) {
        cap  = 8;                         /* initial capacity >= 5                */
        buf  = (seq_t *)malloc(sizeof(seq_t)*cap);
        assert(buf != NULL);
        buf[0] = (seq_t)1; /* a(1) */
        buf[1] = (seq_t)0; /* a(2) */
        buf[2] = (seq_t)1; /* a(3) */
        buf[3] = (seq_t)2; /* a(4) */
        buf[4] = (seq_t)2; /* a(5) */
        size  = 5;
    }

    /* extend storage if needed */
    if (n > size) {
        if (cap < n) {
            idx_t newcap = cap;
            if (newcap < 8) newcap = 8;
            while (newcap < n) newcap <<= 1;      /* double until big enough */
            seq_t *tmp = (seq_t *)realloc(buf, sizeof(seq_t)*newcap);
            assert(tmp != NULL);
            buf = tmp;
            cap = newcap;
        }

        /* compute uncovered terms */
        for (idx_t i = size + 1; i <= n; ++i) {
            seq_t val;
            idx_t k;
            switch (i % 6) {
                case 0:  /* i = 6k   */
                    k = i / 6;
                    val = buf[(2*k-1)-1] + buf[(3*k-1)-1];
                    break;
                case 1:  /* i = 6k+1 */
                    k = (i-1) / 6;
                    val = buf[(3*k-1)-1];
                    break;
                case 2:  /* i = 6k+2 */
                    k = (i-2) / 6;
                    val = buf[(2*k)-1] + buf[(3*k)-1];
                    break;
                case 3:  /* i = 6k+3 */
                    k = (i-3) / 6;
                    val = buf[(2*k)-1] + buf[(3*k)-1];
                    break;
                case 4:  /* i = 6k+4 */
                    k = (i-4) / 6;
                    val = buf[3*k];
                    break;
                case 5:  /* i = 6k+5 */
                    k = (i-5) / 6;
                    val = buf[2*k] + buf[3*k];
                    break;
                default: /* unreachable */
                    val = (seq_t)0;
            }
            buf[i-1] = val;
        }
        size = n;
    }

    return buf[n-1];
}
