/*
 * A294556  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1) + b(n-2) + n + 1, where a(0) = 1, a(1) = 2, b(0) = 3, and
 *          (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A294556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = a(n-1) + a(n-2) + b(n-1) + b(n-2) + n + 1,
   a(0) = 1, a(1) = 2, b(0) = 3,
   b(k) is the smallest positive integer not appearing in
   a(0), …, a(k-1) or b(0), …, b(k-1).          */

#include <stdlib.h>
#include <stdbool.h>

/* return true if val appears in the sorted array arr[0..len-1] */
static bool contains(const seq_t *arr, idx_t len, seq_t val)
{
    idx_t lo = 0, hi = len - 1;
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (arr[mid] == val)
            return true;
        if (arr[mid] < val)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    /* allocate array to hold a[0..n] */
    seq_t *a_vals = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!a_vals)
        return (seq_t)0; /* unlikely, but defensive */

    a_vals[0] = (seq_t)1;
    a_vals[1] = (seq_t)2;

    /* b(0) and b(1) */
    seq_t b_prev2 = (seq_t)3;
    seq_t b_prev1 = (seq_t)4;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* compute a(i) */
        seq_t sum = a_vals[i - 1] + a_vals[i - 2]
                   + b_prev1 + b_prev2
                   + (seq_t)(i) + (seq_t)1;
        a_vals[i] = sum;

        /* find b(i): smallest positive integer > b_prev1 not in a[0..i] */
        seq_t cand = b_prev1 + (seq_t)1;
        while (contains(a_vals, i + 1, cand))
            cand += (seq_t)1;

        /* update b history */
        b_prev2 = b_prev1;
        b_prev1 = cand;
    }

    seq_t result = a_vals[n];
    free(a_vals);
    return result;
}
