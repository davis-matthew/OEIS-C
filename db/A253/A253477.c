/*
 * A253477  Indices of centered heptagonal numbers (A069099) which are also
 *          centered triangular numbers (A005448).
 * https://oeis.org/A253477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1,a(2)=10,a(3)=46,a(4)=1045,a(5)=5005;
   a(n)=a(n-1)+110*a(n-2)-110*a(n-3)-a(n-4)+a(n-5) for n>5 */
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0; /* undefined for offset 0 */

    /* initial values for n=1..5 */
    if (n <= 5) {
        static const seq_t init[5] = { (seq_t)1, (seq_t)10, (seq_t)46,
                                       (seq_t)1045, (seq_t)5005 };
        return init[n - 1];
    }

    /* ring buffer holding the last five terms: a(k-5) .. a(k-1) */
    seq_t buf[5] = { (seq_t)1, (seq_t)10, (seq_t)46, (seq_t)1045, (seq_t)5005 };

    /* Compute terms from k = 6 up to n */
    for (idx_t k = 6; k <= n; ++k) {
        seq_t next = buf[4]                     /* a(k-1) */
                   + (seq_t)110 * buf[3]        /* +110*a(k-2) */
                   - (seq_t)110 * buf[2]        /* -110*a(k-3) */
                   - buf[1]                    /* -a(k-4) */
                   + buf[0];                   /* +a(k-5) */

        /* shift buffer to make room for next term */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = next;
    }

    return buf[4];
}
