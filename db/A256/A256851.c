/*
 * A256851  First-order differences for the cumulative sum of the digits of
 *          the integers when alternately adding and subtracting each digit of
 *          a particular value.
 * https://oeis.org/A256851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base, easy
 * Formula:   a(n) = Sum_{m=1..r(n)} (-1)^(A256100(n,m)+1) * digit(n,m)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the n‑th term of A256851 by counting, for each digit,
   how many times it has appeared in all integers < n, then adding
   or subtracting the digits of n according to the next occurrence
   (odd → add, even → subtract). */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset 1, a(0) is defined as 0 */

    /* counts of each decimal digit seen in 1..n-1 */
    unsigned long cnt[10] = {0};
    seq_t sum = 0;

    /* update counts for all numbers less than n */
    for (idx_t i = 1; i < n; ++i) {
        idx_t x = i;
        /* examine digits left‑to‑right */
        char digs[21];
        int len = 0;
        do {
            digs[len++] = (char)(x % 10);
            x /= 10;
        } while (x);
        for (int k = len - 1; k >= 0; --k) {
            int d = digs[k];
            ++cnt[d];
        }
    }

    /* compute a(n) from the digits of n using the updated counts */
    idx_t x = n;
    char digs[21];
    int len = 0;
    do {
        digs[len++] = (char)(x % 10);
        x /= 10;
    } while (x);

    for (int k = len - 1; k >= 0; --k) {
        int d = digs[k];
        ++cnt[d];
        if (cnt[d] & 1u) {
            sum += (seq_t)d;                /* odd occurrence → add */
        } else {
            sum -= (seq_t)d;                /* even occurrence → subtract */
        }
    }

    return sum;
}
