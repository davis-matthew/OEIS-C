/*
 * A275728  a(0) = 0, for n >= 1, a(n) = A275736(n) + a(A257684(n)); a(n) =
 *          A048675(A275734(n)).
 * https://oeis.org/A275728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(0) = 0, a(n) = A275736(n) + a(A257684(n)) for n >= 1 (iterated
 *            until n reaches 0)
 * Range:     u8 n <= 40320, u16 n <= 40320, u32 n <= 40320, u64 n <= 40320,
 *            u128 n <= 40320, big any n
 * Validated: standard, pass, 116 terms through n = 9824 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 0;
    while (n > 0) {
        unsigned d[24];
        int len = 0;
        idx_t m = n;
        idx_t base = 2;
        while (m > 0) {
            d[len++] = (unsigned)(m % base);   /* d[k-1] = factorial digit k */
            m /= base;
            base++;
        }
        idx_t next = 0;
        idx_t fact = 1;                        /* (k-1)! */
        for (int k = 1; k <= len; ++k) {
            if (d[k - 1] == 1)
                res += (seq_t)1 << (k - 1);    /* A275736 contribution */
            if (k >= 2) {
                if (d[k - 1] > 1)
                    next += (idx_t)(d[k - 1] - 1) * fact;
            }
            if (k >= 2) fact *= (idx_t)k;      /* becomes k! for next index k+1 -> (k)! */
        }
        n = next;
    }
    return res;
}
