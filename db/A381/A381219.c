/*
 * A381219  a(n) = (6^n+2^n-2*3^n)*(n-1)!/2.
 * https://oeis.org/A381219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS sequence A381219: a(n) = (6^n+2^n-2*3^n)*(n-1)!/2
 *
 *  Recurrence (valid for n ≥ 4):
 *      a(n) = 11*(n-1)*a(n-1)
 *             - 36*(n-1)*(n-2)*a(n-2)
 *             + 36*(n-1)*(n-2)*(n-3)*a(n-3)
 *
 *  Initial values (offset 1):
 *      a(1) = 1
 *      a(2) = 11
 *      a(3) = 170
 *
 *  The implementation evaluates the recurrence iteratively using only
 *  the last three already-computed terms; this keeps memory usage
 *  constant and guarantees the correct result for any n ≥ 1, for
 *  arbitrarily-sized types as required by the build.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 1) return 1;
    if (n == 2) return 11;
    if (n == 3) return 170;

    /* arr[0] = a(k-3), arr[1] = a(k-2), arr[2] = a(k-1) at loop start. */
    seq_t arr[3];
    arr[0] = 1;   /* a(1) */
    arr[1] = 11;  /* a(2) */
    arr[2] = 170; /* a(3) */

    /* Iterate from k = 4 up to n, computing a(k) each step. */
    for (idx_t k = 4; k <= n; ++k)
    {
        /* Compute the components of the recurrence. */
        seq_t t1 = 11 * ((seq_t)(k - 1) * arr[2]);          /* 11*(k-1)*a(k-1) */
        seq_t t2 = 36 * ((seq_t)(k - 1) * (k - 2) * arr[1]);/* 36*(k-1)*(k-2)*a(k-2) */
        seq_t t3 = 36 * ((seq_t)(k - 1) * (k - 2) * (k - 3) * arr[0]); /* 36*(k-1)*(k-2)*(k-3)*a(k-3) */

        /* a(k) = t1 - t2 + t3  (all operations are unsigned). */
        seq_t a_k = (t1 + t3) - t2;

        /* Shift the last three values for the next iteration. */
        arr[0] = arr[1];
        arr[1] = arr[2];
        arr[2] = a_k;
    }

    /* After the loop, arr[2] holds a(n). */
    return arr[2];
}
