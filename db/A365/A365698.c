/*
 * A365698  G.f. satisfies A(x) = 1 + x^5 / (1 - x*A(x)).
 * https://oeis.org/A365698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   for n>=0, a(n) = sum_{i=0..n-1} a(i)*a(n-1-i) - a(n-1) * +
 *            (n==0)+(n==5) * Initial values: a(0)=1, a(1)=0, a(2)=0, a(3)=0,
 *            a(4)=0, a(5)=1, ... * This implements OEIS sequence A365698
 * Range:     u8 n <= 21, u16 n <= 33, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation state */
    static seq_t *arr = NULL;          /* array of previously computed values */
    static idx_t  cap = 0;           /* allocated capacity (elements) */
    static idx_t  last = 0;           /* highest index that has been set */

    /* allocate initial space if needed */
    if (cap == 0) {
        arr = (seq_t *)malloc(sizeof(seq_t));
        if (!arr) exit(EXIT_FAILURE);
        arr[0] = (seq_t)1;            /* a(0) = 1 */
        cap = 1;
        last = 0;
    }

    /* grow the array until it can hold a(n) */
    if (n + 1 > cap) {
        idx_t new_cap = cap;
        while (new_cap < n + 1)
            new_cap <<= 1;               /* double until big enough */
        arr = (seq_t *)realloc(arr, new_cap * sizeof(seq_t));
        if (!arr) exit(EXIT_FAILURE);
        cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t k = last + 1; k <= n; ++k) {
        seq_t sum = 0;
        /* convolution part: sum_{i=0}^{k-1} a(i)*a(k-1-i) */
        for (idx_t i = 0; i < k; ++i) {
            sum += arr[i] * arr[k - 1 - i];
        }
        /* subtract a(k-1) if k>0 */
        if (k > 0)
            sum -= arr[k - 1];
        /* add the delta terms */
        if (k == 0)
            sum += (seq_t)1;
        if (k == 5)
            sum += (seq_t)1;
        arr[k] = sum;
    }

    last = n;
    return arr[n];
}
