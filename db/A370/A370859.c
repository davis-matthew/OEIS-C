/*
 * A370859  Numbers m such that c(0) < c(1) > c(2), where c(k) = number of k's
 *          in the ternary representation of m.
 * https://oeis.org/A370859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer m such that, in the base‑3
 *            representation of m, the count of digit 1 exceeds the counts of
 *            both digit 0 and digit 2 (i.e. c(0) < c(1) > c(2)). The function
 *            generates the sequence by scanning the integers in order
 * Range:     u8 n <= 67, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *vals   = NULL;   /* stored sequence values      */
    static size_t  cap   = 0;      /* current allocated capacity  */
    static size_t  len   = 0;      /* number of values stored    */
    static seq_t  next   = 1;      /* next integer to test       */

    /* Expand the stored sequence until it contains at least n terms */
    while (len < n) {
        seq_t  m   = next;          /* candidate integer */
        seq_t  tmp = m;
        size_t c0 = 0, c1 = 0, c2 = 0;

        /* Count digits of m in base 3 */
        while (tmp > 0) {
            int d = (int)(tmp % 3);
            if (d == 0)          ++c0;
            else if (d == 1)     ++c1;
            else                 ++c2;
            tmp /= 3;
        }

        /* Accept if c1 is strictly larger than c0 and c2 */
        if (c1 > c0 && c1 > c2) {
            if (len == cap) {
                cap = cap ? cap * 2 : 16;
                vals = (seq_t *)realloc(vals, cap * sizeof(seq_t));
            }
            vals[len++] = m;
        }

        next = m + 1;              /* advance to next candidate */
    }

    return vals[n - 1];
}
