/*
 * A312484  Coordination sequence Gal.5.81.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4, a(2)=8, a(3)=14, a(4)=18, a(5)=22, a(6)=26,
 *            a(7)=32, a(8)=36, a(9)=40, a(10)=44 a(n)=a(n-1)+a(n-9)-a(n-10)
 *            for n>10
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static void ensure_capacity(idx_t needed, seq_t **arr, size_t *cap)
{
    if (*cap <= needed) {
        size_t new_cap = (*cap) ? (*cap) * 2 : 16;
        while (new_cap <= needed) new_cap *= 2;
        seq_t *new_arr;
#ifdef __cplusplus
        new_arr = new seq_t[new_cap];
        for (size_t i = 0; i < *cap; ++i) new_arr[i] = (*arr)[i];
#else
        new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        memcpy(new_arr, *arr, (*cap) * sizeof(seq_t));
#endif
        /* free the old array */
#ifdef __cplusplus
        delete[] *arr;
#else
        free(*arr);
#endif
        *arr = new_arr;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    static seq_t *seq   = NULL;
    static size_t cap   = 0;
    static idx_t max_c  = (idx_t)-1;
    static int  init    = 0;

    if (!init) {
        /* initial capacity at least 10 */
        ensure_capacity(10, &seq, &cap);

        seq[0] = 1; seq[1] = 4; seq[2] = 8;  seq[3] = 14;
        seq[4] = 18; seq[5] = 22; seq[6] = 26; seq[7] = 32;
        seq[8] = 36; seq[9] = 40; seq[10] = 44;
        max_c = 10;
        init  = 1;
    }

    if (n <= max_c)
        return seq[n];

    ensure_capacity(n, &seq, &cap);

    for (idx_t i = max_c + 1; i <= n; ++i) {
        /* recurrence valid for i > 10 */
        seq[i] = seq[i - 1] + seq[i - 9] - seq[i - 10];
    }

    max_c = n;
    return seq[n];
}
