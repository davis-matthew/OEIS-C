/*
 * A295362  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1) - b(n-2) - b(n-3), where a(0) = 1, a(1) = 3, a(2) = 5, b(0)
 *          = 2, b(1) = 4, b(2) = 6, and (a(n)) and (b(n)) are increasing
 *          complementary sequences.
 * https://oeis.org/A295362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+b(n-1)-b(n-2)-b(n-3), * a(0)=1, a(1)=3,
 *            a(2)=5; * b(0)=2, b(1)=4, b(2)=6; * b(n) is the smallest integer
 *            > b(n-1) that does not occur in * a[0..n] ∪ b[0..n-1]. The
 *            sequences a() and b() are complementary
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static idx_t cache_size = 0;       /* number of already computed terms   */
static seq_t *A = NULL;            /* a[0..cache_size-1]                */
static seq_t *B = NULL;            /* b[0..cache_size-1]                */

/* binary search: return true if x is in arr[0..len-1] */
static bool in_seq(seq_t x, const seq_t *arr, idx_t len)
{
    idx_t lo = 0, hi = len - 1;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (arr[mid] == x) return true;
        if (arr[mid] < x) lo = mid + 1;
        else hi = mid - 1;
    }
    return false;
}

/* compute the next b[n] assuming that A[0..n] and B[0..n-1] are known */
static seq_t next_b(idx_t n)
{
    /* n >= 3 here */
    seq_t cand = B[n-1] + (seq_t)1;
    while (true) {
        if (!in_seq(cand, A, n + 1) && !in_seq(cand, B, n)) {
            return cand;
        }
        cand += (seq_t)1;
    }
}

/* Ensure that the arrays are large enough to store terms up to n */
static void grow_cache(idx_t n)
{
    if (n < cache_size) return;           /* already ready */
    size_t new_size = (size_t)n + 1;      /* need indices 0..n inclusive */
    if (cache_size == 0) {                /* first allocation */
        A = (seq_t *)malloc(new_size * sizeof(seq_t));
        B = (seq_t *)malloc(new_size * sizeof(seq_t));
    } else {
        A = (seq_t *)realloc(A, new_size * sizeof(seq_t));
        B = (seq_t *)realloc(B, new_size * sizeof(seq_t));
    }
    /* initialise newly allocated entries */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            A[0] = (seq_t)1;
            B[0] = (seq_t)2;
        } else if (i == 1) {
            A[1] = (seq_t)3;
            B[1] = (seq_t)4;
        } else if (i == 2) {
            A[2] = (seq_t)5;
            B[2] = (seq_t)6;
        } else {
            /* recurrence for a(i) */
            A[i] = A[i-1] + A[i-2] + B[i-1] - B[i-2] - B[i-3];
            /* next b(i) */
            B[i] = next_b(i);
        }
    }
    cache_size = new_size;
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    grow_cache(n);
    return A[n];
}
