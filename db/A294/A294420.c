/*
 * A294420  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          2*b(n-1) + b(n-2), where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4.
 * https://oeis.org/A294420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*b(n-1) + b(n-2) Initial values: a(0)
 *            = 1, a(1) = 3, b(0) = 2, b(1) = 4 The sequence b(n) is the nth
 *            positive integer that is not equal to any a(k)
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the two trivial cases immediately. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* Allocate space for a and b up to the requested index. */
    seq_t *arr_a = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *arr_b = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!arr_a || !arr_b) return (seq_t)0;  /* unlikely, but safety */

    /* Set the given initials. */
    arr_a[0] = (seq_t)1;          /* a(0) */
    arr_a[1] = (seq_t)3;          /* a(1) */
    arr_b[0] = (seq_t)2;          /* b(0) */
    arr_b[1] = (seq_t)4;          /* b(1) */

    /* The number of a-values known so far. */
    idx_t cur_a_cnt = 2;          /* a[0] and a[1] are known */

    /* Indices used for generating the complement sequence b(n). */
    idx_t ptr_a = 0;              /* next a-value to compare against */
    seq_t cand  = 1;              /* candidate integer for being next b */

    /* Helper macro to produce the next missing number. */
#if defined(__cplusplus)
    #define NEXT_MISSING(res)            \
        do {                             \
            while (ptr_a < cur_a_cnt &&   \
                   cand == arr_a[ptr_a]) {\
                ++cand; ++ptr_a;          \
            }                              \
            res = cand;                   \
            ++cand;                       \
        } while (0)
#else
    #define NEXT_MISSING(res)            \
        do {                             \
            while (ptr_a < cur_a_cnt &&   \
                   cand == arr_a[ptr_a]) {\
                ++cand; ++ptr_a;          \
            }                              \
            res = cand;                   \
            ++cand;                       \
        } while (0)
#endif

    /* Update the state to account for the two explicit b values. */
    NEXT_MISSING(arr_b[0]);   /* advances state past b(0) = 2 */
    NEXT_MISSING(arr_b[1]);   /* advances state past b(1) = 4 */

    /* Compute a(n) and the complementary b(n) for all indices up to n. */
    for (idx_t i = 2; i <= n; ++i) {
        /* Recurrence for a. */
        arr_a[i] = arr_a[i-1] + arr_a[i-2]
                 + (seq_t)2 * arr_b[i-1]
                 + arr_b[i-2];

        /* One more a-value is now known. */
        ++cur_a_cnt;

        /* Generate the next complementary value. */
        NEXT_MISSING(arr_b[i]);
    }

    seq_t result = arr_a[n];

    /* Clean up. */
    free(arr_a);
    free(arr_b);

    return result;
}
