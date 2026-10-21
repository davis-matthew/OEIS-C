/*
 * A251875  Number of (n+2)X(7+2) 0..3 arrays with every 3X3 subblock row and
 *          column sum not 2 4 5 or 7 and every diagonal and antidiagonal sum
 *          2 4 5 or 7.
 * https://oeis.org/A251875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 29*a(n-3) - 168*a(n-6) for n>17. Initial values:
 *            a(1)=32372, a(2)=99808, a(3)=40816, a(4)=62720, a(5)=267872,
 *            a(6)=506656, a(7)=893120, a(8)=3499888, a(9)=8000000,
 *            a(10)=15317248, a(11)=56450688, a(12)=146923792,
 *            a(13)=294439232, a(14)=1049159568, a(15)=2916709968,
 *            a(16)=5965120064, a(17)=...
 * Range:     u8 none, u16 n <= 1, u32 n <= 15, u64 n <= 37, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[17] = {
    (seq_t)32372ULL, (seq_t)99808ULL, (seq_t)40816ULL, (seq_t)62720ULL,
    (seq_t)267872ULL, (seq_t)506656ULL, (seq_t)893120ULL, (seq_t)3499888ULL,
    (seq_t)8000000ULL, (seq_t)15317248ULL, (seq_t)56450688ULL,
    (seq_t)146923792ULL, (seq_t)294439232ULL, (seq_t)1049159568ULL,
    (seq_t)2916709968ULL, (seq_t)5965120064ULL, (seq_t)20941831888ULL
};

seq_t a(idx_t n) {
    if (n == 0) {
        /* The sequence is defined for n ≥ 1; return 0 for n = 0. */
        return 0;
    }
    if (n <= 17) {
        return init_vals[n - 1];
    }
    /* Allocate an array to hold the terms up to n. */
    seq_t *arr = (seq_t*)malloc(n * sizeof(seq_t));
    if (!arr) {
        /* Allocation failed; abort as the sequence cannot be computed. */
        abort();
    }
    /* Copy the initial 17 terms. */
    for (idx_t i = 0; i < 17; ++i) {
        arr[i] = init_vals[i];
    }
    /* Compute remaining terms via the empirical recurrence. */
    for (idx_t i = 18; i <= n; ++i) {
        arr[i - 1] = 29 * arr[i - 4] - 168 * arr[i - 7];
    }
    seq_t result = arr[n - 1];
    free(arr);
    return result;
}
