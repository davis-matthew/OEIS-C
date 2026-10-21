/*
 * A293317  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2) - 1, where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4.
 * https://oeis.org/A293317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, b(0)=2, b(1)=4, a(n)=a(n-1)+a(n-2)+b(n-2)-1 *
 *            The complementary sequence b(n) is the smallest unused positive
 *            integer after b(n-1). * a and b are both strictly increasing, so
 *            we can generate them simultaneously * and find each new b(n) by
 *            scanning the next integer that is not in a...
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* Allocate arrays for a(n) and b(n).  Size n+2 to have indices 0..n for both. */
    size_t sz = (size_t)(n + 2);
    seq_t *a_arr = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *b_arr = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!a_arr || !b_arr) {  /* allocation failure */
        return (seq_t)0;
    }

    a_arr[0] = (seq_t)1;
    a_arr[1] = (seq_t)3;
    b_arr[0] = (seq_t)2;
    b_arr[1] = (seq_t)4;

    /* idx_a is a pointer into a_arr indicating the first a value >= candidate */
    size_t idx_a = 0;

    /* candidate is the next integer to test for being unused; start after b[1] */
    seq_t candidate = b_arr[1] + (seq_t)1;

    for (idx_t k = 2; k <= n; ++k) {
        /* compute a[k] using the recurrence */
        a_arr[k] = a_arr[k-1] + a_arr[k-2] + b_arr[k-2] - (seq_t)1;

        /* compute b[k] by finding the next unused integer > b[k-1] */
        /* candidate starts already as b[k-1] + 1 at beginning of loop */
        while (1) {
            /* advance idx_a until a[idx_a] >= candidate */
            while (idx_a <= (size_t)k && a_arr[idx_a] < candidate) {
                ++idx_a;
            }
            /* if candidate equals an existing a value, skip it */
            if (idx_a <= (size_t)k && a_arr[idx_a] == candidate) {
                ++candidate;
                continue;
            }
            /* otherwise candidate is unused */
            break;
        }
        b_arr[k] = candidate;
        /* increment candidate for next b value */
        candidate = b_arr[k] + (seq_t)1;
    }

    seq_t result = a_arr[n];
    free(a_arr);
    free(b_arr);
    return result;
}
