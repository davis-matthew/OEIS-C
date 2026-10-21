/*
 * A252362  Number of (n+2) X (1+2) 0..3 arrays with every 3 X 3 subblock row,
 *          column, diagonal and antidiagonal sum not equal to 1 3 4 6 or 7.
 * https://oeis.org/A252362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-3) + 4*a(n-4) + 4*a(n-5) + 2*a(n-6), n>8
 *            Initial values: a(1)=130, a(2)=152, a(3)=282, a(4)=658,
 *            a(5)=1482, a(6)=3188, a(7)=6954, a(8)=15432
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 23, u64 n <= 51, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* memoisation storage: data[0] holds a(1), data[1] holds a(2), etc. */
    static seq_t *data = NULL;
    static idx_t max_n = 0;          /* number of values currently stored */

    /* initial values for n = 1..8 */
    static const seq_t base[8] = {
        (seq_t)130, (seq_t)152, (seq_t)282, (seq_t)658,
        (seq_t)1482, (seq_t)3188, (seq_t)6954, (seq_t)15432
    };

    if (n == 0)
        return (seq_t)0;        /* undefined for offset 1, but safe return */

    if (n <= 8)
        return base[n - 1];

    /* need to extend memoisation array up to n */
    if (n > max_n) {
        idx_t new_size = n;
        if (data == NULL) {
            data = (seq_t *)malloc(new_size * sizeof(seq_t));
        } else {
            data = (seq_t *)realloc(data, new_size * sizeof(seq_t));
        }
        if (data == NULL)
            abort();              /* memory allocation failed */

        /* initialise first eight entries */
        for (idx_t i = 0; i < 8; ++i)
            data[i] = base[i];

        /* compute remaining values up to n */
        for (idx_t i = 8; i < new_size; ++i) {   /* i = 8 means a(9) */
            seq_t val = data[i - 1];   /* a(n-1)   */
            val += 3 * data[i - 3];    /* +3*a(n-3) */
            val += 4 * data[i - 4];    /* +4*a(n-4) */
            val += 4 * data[i - 5];    /* +4*a(n-5) */
            val += 2 * data[i - 6];    /* +2*a(n-6) */
            data[i] = val;
        }

        max_n = new_size;
    }

    return data[n - 1];
}
