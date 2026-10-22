/*
 * A348310  a(n) = Sum_{k=0..floor(n/10)} (-1)^k * binomial(n-5*k,5*k).
 * https://oeis.org/A348310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) -
 *            a(n-10) Initial values: a(0)=1, a(1)=1, …, a(9)=1, a(10)=0 This
 *            implements the recurrence directly, computing values up to n on
 *            each call
 * Range:     i8 n <= 14, i16 n <= 28, i32 n <= 49, i64 n <= 100,
 *            i128 n <= 193, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle small indices directly */
    if (n <= 9)
        return 1;          /* a(0) … a(9) = 1 */
    if (n == 10)
        return 0;          /* a(10) = 0 */

    /* Allocate an array large enough to hold all needed values */
    size_t len = (size_t)n + 1;
    seq_t *arr = (seq_t *)calloc(len, sizeof(seq_t));
    if (!arr)
        return 0;          /* allocation failure – unlikely for n≤10000 */

    /* Initialise base values */
    for (idx_t i = 0; i <= 9; ++i)
        arr[(size_t)i] = 1;
    arr[10] = 0;

    /* Build the sequence up to n using the recurrence */
    for (idx_t i = 11; i <= n; ++i) {
        size_t idx = (size_t)i;
        arr[idx] = (seq_t)5   * arr[idx-1]
                 - (seq_t)10  * arr[idx-2]
                 + (seq_t)10  * arr[idx-3]
                 - (seq_t)5   * arr[idx-4]
                 +            arr[idx-5]
                 -            arr[idx-10];
    }

    seq_t result = arr[(size_t)n];
    free(arr);
    return result;
}
