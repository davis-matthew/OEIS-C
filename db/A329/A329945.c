/*
 * A329945  Number of permutations of [n] whose cycle lengths avoid squares.
 * https://oeis.org/A329945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=0, a(n)=Σ_{k=1..n, k not a perfect square}
 *            P(n-1,k-1)*a(n-k) where P(n-1,k-1)=∏_{t=1}^{k-1}(n-t)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* integer floor sqrt for idx_t (values up to 10 000 are typical). */
static idx_t int_sqrt(idx_t n)
{
    idx_t left = 0, right = n;
    while (left <= right) {
        idx_t mid = left + ((right - left) >> 1);
        idx_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return left - 1;
}

/* check whether i is a perfect square */
static bool is_square(idx_t i)
{
    idx_t r = int_sqrt(i);
    return r * r == i;
}

/* main sequence function */
seq_t a(idx_t n)
{
    static seq_t *A = NULL;   /* a[0..cap-1] */
    static bool *IS = NULL;   /* IS[k] true iff k is a perfect square */
    static idx_t cap = 0;     /* current capacity */

    if (n >= cap) {
        idx_t new_cap = n + 1;
        A = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        IS = (bool *)realloc(IS, new_cap * sizeof(bool));

        for (idx_t i = cap; i < new_cap; ++i) {
            IS[i] = is_square(i);
            if (i == 0)
                A[0] = 1;
        }

        for (idx_t i = cap; i < new_cap; ++i) {
            if (i == 0) continue;
            seq_t sum = 0;
            seq_t r   = 1;          /* Π_{t=1}^{k-1} (i - t) */

            for (idx_t k = 1; k <= i; ++k) {
                if (!IS[k])         /* k is not a perfect square */
                    sum += r * A[i - k];
                if (k < i)          /* update r for next k */
                    r *= (seq_t)(i - k);
            }
            A[i] = sum;
        }
        cap = new_cap;
    }
    return A[n];
}
