/*
 * A335562  Number of unlabelled unary-binary trees with n nodes such that
 *          every node with two children has children of different subtree
 *          sizes.
 * https://oeis.org/A335562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(1) = 1 * For n >= 2: * a(n) = a(n-1) + * sum_{k=1}^{n-2}
 *            a(k) * a(n-k-1) - * [n odd] * a((n-1)/2)^2 * ( [n odd] is 1 when
 *            n is odd, else 0 ) * * Example terms: a(1)=1, a(2)=1, a(3)=1,
 *            a(4)=3, a(5)=5,
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 26, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Memoization cache */
    static seq_t *A = NULL;     /* array of a[0..cap] (a[0] unused) */
    static idx_t cap = 0;       /* current allocated capacity */

    if (n == 0) return 0;     /* offset is 1; value for 0 is unused */

    /* Initialize cache for first call */
    if (cap == 0) {
        cap = 1;                                 /* need index 1 */
        A = (seq_t *)malloc((cap + 1) * sizeof(seq_t));
        if (!A) exit(1);
        A[0] = (seq_t)0;                         /* unused */
        A[1] = (seq_t)1;                         /* a(1) */
    }

    /* Grow cache if needed */
    if (n > cap) {
        idx_t new_cap = cap;
        while (new_cap < n) new_cap <<= 1;       /* double until enough */

        seq_t *tmp = (seq_t *)realloc(A, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) exit(1);
        A = tmp;

        /* Compute new terms sequentially */
        for (idx_t i = cap + 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;

            /* Sum over all ordered pairs (k, i-k-1) */
            for (idx_t k = 1; k <= i - 2; ++k) {
                sum += A[k] * A[i - k - 1];
            }

            /* Exclude term where subtree sizes are equal (if i is odd) */
            if ((i & 1U) == 1U) {                /* i odd */
                idx_t mid = (i - 1U) >> 1U;      /* (i-1)/2 */
                sum -= A[mid] * A[mid];
            }

            A[i] = A[i - 1] + sum;
        }

        cap = n;
    }

    return A[n];
}
