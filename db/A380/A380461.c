/*
 * A380461  Number of edge covers of fan graph F_{2,n}.
 * https://oeis.org/A380461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - 24*a(n-2) - 21*a(n-3) + 33*a(n-4) + 34*a(n-5)
 *            + 8*a(n-6), a(1)=1, a(2)=16, a(3)=154, a(4)=1289, a(5)=10180,
 *            a(6)=78372
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    static seq_t *arr = NULL;
    static size_t cap = 0;
    static size_t sz = 0;   // number of values already stored (arr[0] = a(1))

    /* Ensure the array is large enough */
    if ((size_t)n > cap) {
        size_t newcap = (cap == 0) ? 16 : cap << 1;
        while (newcap < (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(arr, newcap * sizeof(seq_t));
        arr = tmp;
        cap = newcap;
    }

    /* Initialize base values if not already in the cache */
    if (sz < 6) {
        if (sz < 1) { arr[0] = 1; sz = 1; }
        if (sz < 2) { arr[1] = 16; sz = 2; }
        if (sz < 3) { arr[2] = 154; sz = 3; }
        if (sz < 4) { arr[3] = 1289; sz = 4; }
        if (sz < 5) { arr[4] = 10180; sz = 5; }
        if (sz < 6) { arr[5] = 78372; sz = 6; }
    }

    if ((size_t)n <= sz) return arr[(size_t)(n - 1)];

    /* Compute missing values up to n */
    for (idx_t i = (idx_t)(sz + 1); i <= n; ++i) {
        seq_t term1 = 11 * arr[(size_t)(i - 2)];
        seq_t sub1  = 24 * arr[(size_t)(i - 3)];
        seq_t sub2  = 21 * arr[(size_t)(i - 4)];
        seq_t add1  = 33 * arr[(size_t)(i - 5)];
        seq_t add2  = 34 * arr[(size_t)(i - 6)];
        seq_t add3  = 8  * arr[(size_t)(i - 7)];

        seq_t val = term1 - sub1;
        val -= sub2;
        val += add1;
        val += add2;
        val += add3;

        arr[(size_t)(i - 1)] = val;
    }

    sz = (size_t)n;
    return arr[(size_t)(n - 1)];
}
