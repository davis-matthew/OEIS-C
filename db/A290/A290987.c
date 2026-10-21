/*
 * A290987  Expansion of (1 - 2*x + x^2 - x^4 + x^3 + x^5)/((1 - x)^2*(1 - 2*x
 *          + x^3 - x^4)).
 * https://oeis.org/A290987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A059633(n+2) + A059633(n+3) - n (equivalently: a(n) =
 *            4*a(n-1) - 5*a(n-2) + a(n-3) + 3*a(n-4) - 3*a(n-5) + a(n-6) for
 *            n ≥ 6)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 34, u64 n <= 70, u128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Initial values a(0) … a(5) */
static const seq_t init[6] = {
    (seq_t)1, (seq_t)2, (seq_t)4,
    (seq_t)8, (seq_t)16, (seq_t)32
};

/* Compute the sequence term a(n) */
seq_t a(idx_t n)
{
    /* Small n: return pre‑computed value */
    if (n < 6) {
        return init[n];
    }

    /* Allocate an array of size n+1 to hold all terms up to n */
    size_t sz = (size_t)(n + 1);
    seq_t *arr = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!arr) {                 /* allocation failure – return 0 as a safe fallback */
        return (seq_t)0;
    }

    /* Store the initial terms */
    arr[0] = init[0];
    arr[1] = init[1];
    arr[2] = init[2];
    arr[3] = init[3];
    arr[4] = init[4];
    arr[5] = init[5];

    /* Iteratively apply the linear recurrence */
    for (idx_t i = 6; i <= n; ++i) {
        arr[i] = 4 * arr[i-1]      /* +5*a(n-2) is subtracted last        */
                - 5 * arr[i-2]
                + arr[i-3]
                + 3 * arr[i-4]
                - 3 * arr[i-5]
                + arr[i-6];
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
