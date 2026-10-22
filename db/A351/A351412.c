/*
 * A351412  a(1) = 1, a(2) = 2, a(3) = 3. Then if n is even a(n) is the least
 *          positive integer not yet in the sequence, otherwise if n is odd
 *          a(n) = a(n-1) + a(n-3).
 * https://oeis.org/A351412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; a(2)=2; a(3)=3. For n odd (n≥5): a(n)=3*((n-1)/2). For n
 *            even (n≥4): if n%4==0: a(n)=3*(n/4)+1; if n%4==2:
 *            a(n)=3*((n-2)/4)+2
 * Range:     u8 n <= 172, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* handle the few exceptional initial values */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;

    /* odd indices (n >= 5) */
    if (n & 1U) { /* n is odd */
        seq_t m = (seq_t)(n - 1) / 2;   /* (n-1)/2 fits in idx_t */
        return (seq_t)3 * m;
    }

    /* even indices (n >= 4) */
    if ((n & 3U) == 0) { /* n % 4 == 0 */
        seq_t m = (seq_t)(n / 4);   /* n/4 fits in idx_t */
        return (seq_t)3 * m + (seq_t)1;
    } else { /* n % 4 == 2 */
        seq_t m = (seq_t)((n - 2) / 4);
        return (seq_t)3 * m + (seq_t)2;
    }
}
