/*
 * A384772  Triangle read by row: T(n,k) is the number of the k-th eliminated
 *          person in a variant of the Josephus problem in which first three
 *          people are skipped, then one is eliminated, repeating until all n
 *          people are eliminated.
 * https://oeis.org/A384772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A384772: Josephus elimination with k = 4 (skip 3, kill 1)
   The sequence a(n) is the concatenation of the elimination orders
   for n = 1,2,3,…  (row r has r elements: T(r,1),…,T(r,r)).
   The function below computes a(n) for any n ≥ 1. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Compute the row number r such that r*(r+1)/2 ≥ n > (r-1)*r/2. */
static idx_t
row_for_index(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;
    /* grow high until it covers n */
    while ((high * (high + 1)) / 2 < n)
        high <<= 1;
    /* binary search */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((mid * (mid + 1)) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* offset starts at 1 */

    /* --- locate the row r and the column k within that row --- */
    idx_t r = row_for_index(n);
    idx_t prev_total = (r - 1) * r / 2;
    idx_t k = n - prev_total;          /* 1‑based position in the row */

    /* --- generate elimination order for r people, stop at k‑th elimination --- */
    idx_t *arr = (idx_t *)malloc((size_t)r * sizeof(idx_t));
    if (!arr) return (seq_t)0;           /* allocation failure */

    for (idx_t i = 0; i < r; ++i) arr[i] = i + 1;  /* persons labelled 1..r */

    idx_t current = 0;                   /* index of starting point in arr */
    idx_t remaining = r;
    seq_t result = 0;

    for (idx_t step = 1; step <= k; ++step) {
        /* index of the person to eliminate */
        idx_t kill = (current + 3) % remaining;     /* step size = 4 -> skip 3 */

        result = (seq_t)arr[kill];
        if (step == k) break;                    /* we already have the answer */

        /* remove the eliminated person from the array */
        for (idx_t j = kill; j < remaining - 1; ++j)
            arr[j] = arr[j + 1];
        remaining--;
        /* next round starts at the position where the killed person was */
        current = kill % remaining;
    }

    free(arr);
    return result;
}
