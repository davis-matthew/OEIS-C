/*
 * A308747  Number of achiral m-color cyclic compositions of n (that is,
 *          number of cyclic compositions of n with reflection symmetry where
 *          each part of size m can be colored with one of m colors).
 * https://oeis.org/A308747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-2) - a(n-4) + 2*(n-2), n >= 5 Initial values:
 *            a(1)=1, a(2)=3, a(3)=6, a(4)=13
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* assume seq_t and idx_t are defined elsewhere (uint types) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle the first four values directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)13;

    /* allocate an array to hold the sequence up to n */
    seq_t *arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!arr) return (seq_t)0;   /* fallback on out‑of‑memory */

    arr[1] = (seq_t)1;
    arr[2] = (seq_t)3;
    arr[3] = (seq_t)6;
    arr[4] = (seq_t)13;

    for (idx_t i = 5; i <= n; ++i) {
        seq_t temp = 3 * arr[i - 2];
        temp -= arr[i - 4];
        temp += 2 * ((seq_t)(i - 2));
        arr[i] = temp;
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
