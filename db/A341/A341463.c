/*
 * A341463  a(n) = (-1)^(n+1) * (3^n+1)/2.
 * https://oeis.org/A341463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^(n+1) * (3^n + 1) / 2
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 19, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* Cache of powers of 3: pow3[i] = 3^i */
    static seq_t *pow3 = NULL;
    static idx_t pow3_cap = 0;   /* current size of pow3 */

    /* Expand cache if needed */
    if (n >= pow3_cap) {
        idx_t new_cap = n + 1;   /* need indices 0..n inclusive */
        seq_t *tmp = (seq_t *)realloc(pow3, new_cap * sizeof(seq_t));
        assert(tmp);              /* abort on OOM */
        pow3 = tmp;
        for (idx_t i = pow3_cap; i < new_cap; ++i) {
            if (i == 0) {
                pow3[i] = (seq_t)1;           /* 3^0 = 1 */
            } else {
                pow3[i] = pow3[i-1] * (seq_t)3;
            }
        }
        pow3_cap = new_cap;
    }

    /* Compute (3^n + 1) / 2 */
    seq_t part = (pow3[n] + (seq_t)1) / (seq_t)2;

    /* Apply sign (-1)^(n+1) */
    seq_t sign = (((n + (idx_t)1) & (idx_t)1) ? (seq_t)-1 : (seq_t)1);

    return sign * part;
}
