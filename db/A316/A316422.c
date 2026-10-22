/*
 * A316422  Number of n X 3 0..1 arrays with every element unequal to 0, 1, 3,
 *          5, 6 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)
 *            +5*a(n-3)+2*a(n-5)-2*a(n-6)-a(n-7)-2*a(n-8)-3*a(n-9)-2*a(n-11)-a(n-12);
 *            Initial values: a(1)=3, a(2)=9, a(3)=14, a(4)=32, a(5)=75,
 *            a(6)=148, a(7)=311, a(8)=687, a(9)=1443, a(10)=3042, a(11)=6534,
 *            a(12)=13887. Computation is memoised; each term is computed only
 *            once, using the recurr...
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 28, u64 n <= 58, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle trivial case */
    if (n == 0)
        return 0;

    /* Memoisation buffer */
    static seq_t *buf = NULL;
    static size_t buf_cap = 0;   /* allocated size */
    static size_t buf_len = 0;   /* number of terms stored (>= n when computed) */

    /* Initial values for n = 1..12 */
    static const seq_t init[12] = {
        3, 9, 14, 32, 75, 148, 311, 687, 1443, 3042, 6534, 13887
    };

    /* Allocate buffer on first call or increase if needed */
    if (buf == NULL) {
        buf_cap = 12;
        buf = (seq_t *)malloc(buf_cap * sizeof(seq_t));
    }
    if (n > buf_cap) {
        size_t new_cap = buf_cap * 2;
        if (new_cap < n)
            new_cap = n;
        buf = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        buf_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t i = buf_len + 1; i <= (size_t)n; ++i) {
        size_t idx = i - 1;   /* 0‑based index for a(i) */

        if (i <= 12) {
            buf[idx] = init[i - 1];
        } else {
            /* Recurrence:
               a(i) = a(i-1) +5*a(i-3)+2*a(i-5)
                      -2*a(i-6)-a(i-7)-2*a(i-8)-3*a(i-9)
                      -2*a(i-11)-a(i-12)                       */
            seq_t res = buf[idx - 1];                 /* a(i-1)   */
            res += 5 * buf[idx - 3];                  /* a(i-3)   */
            res += 2 * buf[idx - 5];                  /* a(i-5)   */
            res -= 2 * buf[idx - 6];                  /* a(i-6)   */
            res -= buf[idx - 7];                      /* a(i-7)   */
            res -= 2 * buf[idx - 8];                  /* a(i-8)   */
            res -= 3 * buf[idx - 9];                  /* a(i-9)   */
            res -= 2 * buf[idx -11];                  /* a(i-11)  */
            res -= buf[idx -12];                      /* a(i-12)  */
            buf[idx] = res;
        }
    }

    buf_len = n;
    return buf[n - 1];
}
