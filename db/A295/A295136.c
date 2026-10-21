/*
 * A295136  Solution of the complementary equation a(n) = 3*a(n-1) + b(n-1) -
 *          3, where a(0) = 1, a(1) = 2, b(0) = 3, and (a(n)) and (b(n)) are
 *          increasing complementary sequences.
 * https://oeis.org/A295136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2; for n≥2, a(n) = 3*a(n-1)+b(n-1)-3 where b(n) is
 *            the complementary sequence to a(n) (not used directly in the
 *            recurrence except for b(n-1))
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Binary search to test if val occurs in the sorted array arr[0..len-1]. */
static bool contains(const seq_t *arr, size_t len, seq_t val)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = lo + ((hi - lo) >> 1);
        if (arr[mid] < val)
            lo = mid + 1;
        else if (arr[mid] > val)
            hi = mid;
        else
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)2;

    /* Allocate array for a[0..n] */
    seq_t *A = (seq_t*)malloc(((size_t)n + 1u) * sizeof(seq_t));
    if (!A) return (seq_t)0;  /* allocation failure (should not happen) */

    A[0] = (seq_t)1;
    A[1] = (seq_t)2;

    /* The first b value needed by the recurrence is b(1)=4. */
    seq_t next_missing = (seq_t)4;

    for (size_t i = 2; i <= (size_t)n; ++i) {
        seq_t b_val = next_missing;                        /* b(i-1) */
        /* a(i) = 3*a(i-1) + b(i-1) - 3 */
        A[i] = (seq_t)3 * A[i-1] + b_val - (seq_t)3;

        /* Prepare next_missing for the following step. */
        ++next_missing;                                    /* candidate */
        /* Skip any value that equals a previously computed a(j). */
        while (contains(A, i + 1u, next_missing))
            ++next_missing;
    }

    seq_t result = A[n];
    free(A);
    return result;
}
