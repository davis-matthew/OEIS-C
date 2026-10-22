/*
 * A309711  Sum of the even parts appearing among the smallest parts of the
 *          partitions of n into 4 parts.
 * https://oeis.org/A309711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the sum of the even smallest parts among all partitions
 *            of n into 4 parts. The recurrence from the OEIS entry is used:
 *            a(n) =
 *            a(n-1)+a(n-2)-a(n-4)-a(n-5)+a(n-6)+2*a(n-8)-2*a(n-9)-2*a(n-10)
 *            +2*a(n-12)+2*a(n-13)-2*a(n-14)-a(n-16)+a(n-17)+a(n-18)
 *            -a(n-20)-a(n-21)+a(n-22) for n>21, with initi...
 * Range:     u8 n <= 30, u16 n <= 128, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *values = NULL;
    static size_t max_computed = 0;

    /* initial values up to a(22) */
    static const seq_t init[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        2, 2, 4, 6, 8, 10,
        14, 16, 24, 28, 36, 44,
        54, 62, 76
    };
    const size_t init_len = sizeof(init) / sizeof(init[0]);

    /* make sure we have room for values[0..n] */
    if (n >= max_computed) {
        size_t new_size = (size_t)n + 1;
        if (!values) {
            values = (seq_t *)malloc(new_size * sizeof(seq_t));
            memset(values, 0, new_size * sizeof(seq_t));
        } else if (new_size > max_computed) {
            values = (seq_t *)realloc(values, new_size * sizeof(seq_t));
        }

        /* compute missing values */
        for (size_t i = max_computed; i <= (size_t)n; ++i) {
            if (i < init_len) {
                values[i] = init[i];
            } else {
                /* recurrence */
                seq_t tmp = values[i-1] + values[i-2];
                tmp -= values[i-4];
                tmp -= values[i-5];
                tmp += values[i-6];
                tmp += (seq_t)2 * values[i-8];
                tmp -= (seq_t)2 * values[i-9];
                tmp -= (seq_t)2 * values[i-10];
                tmp += (seq_t)2 * values[i-12];
                tmp += (seq_t)2 * values[i-13];
                tmp -= (seq_t)2 * values[i-14];
                tmp -= values[i-16];
                tmp += values[i-17];
                tmp += values[i-18];
                tmp -= values[i-20];
                tmp -= values[i-21];
                tmp += values[i-22];
                values[i] = tmp;
            }
        }
        max_computed = new_size;
    }

    return values[n];
}
