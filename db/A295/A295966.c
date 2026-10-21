/*
 * A295966  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n) - 1, where a(0) = 1, a(1) = 5, b(0) = 2, b(1) = 3, b(2) = 4,
 *          and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n) - 1; a(0) = 1, a(1) = 5; b(0) = 2,
 *            b(1) = 3, b(2) = 4; (a(n)) and (b(n)) are increasing
 *            complementary sequences
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static state: sequences, capacity and generation counter */
static seq_t *A = NULL;
static seq_t *B = NULL;
static size_t  stride = 0;          /* current capacity of A and B   */
static idx_t   last_computed = (idx_t)-1;   /* highest index already stored */
static idx_t   next_candidate = 1;           /* next integer to test for b */

/* Ensure capacity for at least n+1 elements */
static void ensure_capacity(idx_t n)
{
    if (stride >= (size_t)(n + 1))
        return;
    size_t new_stride = stride ? stride << 1 : 8;
    while (new_stride <= (size_t)(n + 1))
        new_stride <<= 1;
    A = (seq_t *)realloc(A, new_stride * sizeof(seq_t));
    B = (seq_t *)realloc(B, new_stride * sizeof(seq_t));
    stride = new_stride;
}

/* Binary search: is 'val' present in A[0..upto-1]? */
static bool present_in_a(seq_t val, idx_t upto)
{
    idx_t lo = 0, hi = upto;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        seq_t m = A[mid];
        if (m == val)
            return true;
        if (m < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

/* Public function required by the problem */
seq_t a(idx_t n)
{
    /* Compute all terms up to n if necessary */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        ensure_capacity(i);

        if (i == 0) {
            /* Initial conditions */
            A[0] = (seq_t)1;
            B[0] = (seq_t)2;
            next_candidate = 3;
            last_computed = 0;
            continue;
        }

        if (i == 1) {
            A[1] = (seq_t)5;
            B[1] = (seq_t)3;
            next_candidate = 4;
            last_computed = 1;
            continue;
        }

        /* Determine b(i): smallest integer > previous b not in a[0..i-1] */
        seq_t cand = (seq_t)next_candidate;
        while (present_in_a(cand, i)) {
            ++cand;
            ++next_candidate;
        }
        B[i] = cand;
        next_candidate = (idx_t)(cand + 1);

        /* Compute a(i) using the recurrence */
        A[i] = A[i - 1] + A[i - 2] + B[i] - 1;

        last_computed = i;
    }
    return A[n];
}
