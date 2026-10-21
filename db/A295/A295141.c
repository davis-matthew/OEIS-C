/*
 * A295141  Solution of the complementary equation a(n) = 2*a(n-1) + a(n-2) +
 *          b(n-2), where a(0) = 1, a(1) = 2, b(0) = 3, b(1) = 4, and (a(n))
 *          and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + b(n-2) Initial values: a(0)=1,
 *            a(1)=2; b(0)=3, b(1)=4. b(n) is the smallest positive integer
 *            not in {a[0..n],b[0..n-1]}
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Binary search: return true if val occurs in the first 'cnt' elements of arr */
static bool contains_a(idx_t cnt, const seq_t *arr, seq_t val)
{
    idx_t low = 0, high = cnt;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (arr[mid] < val)
            low = mid + 1;
        else
            high = mid;
    }
    return (low < cnt) && (arr[low] == val);
}

/* Memoized-computed sequence */
seq_t a(idx_t n)
{
    /* Persistent cache */
    static seq_t *a_arr = NULL;
    static seq_t *b_arr = NULL;
    static idx_t size   = 0;      /* allocated array size */
    static idx_t last_n = 0;      /* highest index computed */
    static seq_t candidate = 0;   /* smallest unused positive integer */

    /* One-time initialization with the first two terms. */
    if (size == 0) {
        size = 2;                         /* indices 0 and 1 */
        a_arr = (seq_t *)malloc(size * sizeof(seq_t));
        b_arr = (seq_t *)malloc(size * sizeof(seq_t));

        a_arr[0] = 1;
        a_arr[1] = 2;
        b_arr[0] = 3;
        b_arr[1] = 4;

        /* Next unused integer after the initial values. */
        candidate = 5;
        last_n = 1;
    }

    /* Compute terms up to the requested index. */
    while (n > last_n) {
        idx_t new_idx = last_n + 1;   /* index of the term to compute */

        /* Ensure the arrays are large enough. */
        if (new_idx + 1 > size) {
            idx_t new_size = size * 2;
            if (new_size < new_idx + 1)
                new_size = new_idx + 1;
            a_arr = (seq_t *)realloc(a_arr, new_size * sizeof(seq_t));
            b_arr = (seq_t *)realloc(b_arr, new_size * sizeof(seq_t));
            size = new_size;
        }

        /* a[new_idx] = 2*a[new_idx-1] + a[new_idx-2] + b[new_idx-2] */
        seq_t tmp = a_arr[new_idx-1] + a_arr[new_idx-1]; /* 2*a[n-1] */
        tmp += a_arr[new_idx-2];
        tmp += b_arr[new_idx-2];
        a_arr[new_idx] = tmp;

        /* Find the next unused integer for b[new_idx]. */
        while (contains_a(new_idx + 1, a_arr, candidate))
            candidate += 1;              /* skip numbers already in a */

        b_arr[new_idx] = candidate;      /* assign to b */
        candidate += 1;                  /* next candidate */

        last_n = new_idx;
    }

    return a_arr[n];
}
