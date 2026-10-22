/*
 * A360211  a(n) = Sum_{k=0..floor(n/2)} (-1)^k * binomial(2*n-3*k,n-2*k).
 * https://oeis.org/A360211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A360211 defined by the recurrence:
 *  2*n*a(n) = (5*n-2)*a(n-1) + (11*n-12)*a(n-2) + 2*(n-5)*a(n-3)
 *             + (7*n-2)*a(n-4) + 2*(2*n-5)*a(n-5)
 * with initial values a(0)=1, a(1)=2, a(2)=5, a(3)=17, a(4)=61
 */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *buf   = NULL;
    static size_t  len  = 0;

    /* initial five terms */
    if (len == 0) {
        len = 5;
        buf = (seq_t *)malloc(len * sizeof(seq_t));
        if (!buf) exit(1);
        buf[0] = (seq_t)1;
        buf[1] = (seq_t)2;
        buf[2] = (seq_t)5;
        buf[3] = (seq_t)17;
        buf[4] = (seq_t)61;
    }

    if (n < 5)               /* pre‑computed values */
        return buf[n];

    /* extend buffer and compute missing terms up to n */
    if (n >= len) {
        size_t old_len = len;
        size_t new_len = n + 1;

        seq_t *tmp = (seq_t *)realloc(buf, new_len * sizeof(seq_t));
        if (!tmp) exit(1);
        buf = tmp;

        for (idx_t i = (idx_t)old_len; i <= n; ++i) {
            seq_t term1 = (seq_t)(5*i - 2) * buf[i-1];
            seq_t term2 = (seq_t)(11*i - 12) * buf[i-2];
            seq_t term3 = (seq_t)(2*(i-5)) * buf[i-3];
            seq_t term4 = (seq_t)(7*i - 2) * buf[i-4];
            seq_t term5 = (seq_t)(2*(2*i - 5)) * buf[i-5];

            seq_t num = term1 + term2 + term3 + term4 + term5;
            seq_t denom = (seq_t)(2*i);
            buf[i] = num / denom;          /* division is exact */
        }
        len = new_len;
    }

    return buf[n];
}
