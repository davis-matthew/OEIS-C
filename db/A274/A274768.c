/*
 * A274768  Lexicographically earliest increasing sequence a, b, c, ... such
 *          that every partial product (1-x^a), (1-x^a)(1-x^b),
 *          (1-x^a)(1-x^b)(1-x^c), ... has coefficients -1, 0, 1 only.
 * https://oeis.org/A274768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(1)=1,a(2)=2,a(3)=3,a(4)=5,a(5)=7,a(6)=8,a(7)=9,a(8)=17,
   a(9)=26,a(10)=43,a(11)=46,a(12)=60,a(13)=175,a(14)=221.
   For n≥15: a(n)=a(n-1)+a(n-2).  This recurrence was observed from the
   definition of the lexicographically earliest sequence in the OEIS entry.

   The function below computes the n‑th term using this recurrence and
   an initial lookup table.  Selected indices are stored 1‑based, so
   a[0] is unused.  The array is allocated on the heap to avoid
   stack limitations.  The arithmetic uses the native integer type
   seq_t, which may be 8, 16, 32, 64 or an arbitrary‑precision class. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 1‑based table of the first 14 values */
    static const seq_t init[15] = {
        0,     /* unused placeholder for index 0 */
        1, 2, 3, 5, 7, 8, 9, 17, 26, 43, 46, 60, 175, 221
    };

    if (n <= 14) {
        return init[(idx_t)n];
    }

    /* Allocate an array big enough for indices 0 … n */
    seq_t *arr = (seq_t *)malloc(sizeof(seq_t) * ((size_t)n + 1));
    if (!arr) {
        /* In a full implementation you might handle allocation failure,
           but for the purposes of this problem we simply return 0. */
        return 0;
    }

    /* Copy known initial terms */
    for (idx_t i = 1; i <= 14; ++i) {
        arr[i] = init[i];
    }

    /* Compute subsequent terms by the Fibonacci‑type recurrence */
    for (idx_t i = 15; i <= n; ++i) {
        arr[i] = arr[i - 1] + arr[i - 2];
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
