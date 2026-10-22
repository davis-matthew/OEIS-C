/*
 * A352762  Expansion of Product_{k>=1} 1 / (1 + 3^(k-1)*x^k).
 * https://oeis.org/A352762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 21, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k=0..n} (-1)^k * p(n,k) * 3^(n-k)
 * where p(n,k) is the number of partitions of n into k parts.
 *
 * The algorithm uses the identity
 *   F_k(x) = 1 / (1 + 3^(k-1) x^k)
 *   = sum_{m>=0} (-1)^m * 3^{m*(k-1)} * x^{m*k}
 *
 * Let dp_prev[s] be the coefficient of x^s after processing factors for
 * k = 1..(current-1).  For the next factor k we compute
 *
 *   dp_new[s] = dp_prev[s] + (-3^(k-1)) * dp_new[s-k]   (s >= k)
 *   dp_new[s] = dp_prev[s]                             (s < k)
 *
 * This recurrence follows from
 *   dp_new[s] = sum_{m>=0} (-3^{(k-1)})^m * dp_prev[s - m*k]
 * and hence is O(N^2) time and O(N) memory.
 */

#include <stddef.h>   /* for idx_t / seq_t types */
#ifndef __cplusplus
#include <stdlib.h>   /* for malloc/free */
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Pre‑compute powers of 3 up to exponent n */
#ifndef __cplusplus
    seq_t *pow3 = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    pow3[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) pow3[i] = pow3[i - 1] * (seq_t)3;
#else
    std::vector<seq_t> pow3(n + 1);
    pow3[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) pow3[i] = pow3[i - 1] * (seq_t)3;
#endif

    /* Allocate two coefficient arrays of size n+1 */
#ifndef __cplusplus
    seq_t *dp_prev = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *dp_new  = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    dp_prev[0] = (seq_t)1;  /* coefficient for x^0 */
    for (idx_t i = 1; i <= n; ++i) dp_prev[i] = (seq_t)0;
#else
    std::vector<seq_t> dp_prev(n + 1, (seq_t)0);
    std::vector<seq_t> dp_new(n + 1);
    dp_prev[0] = (seq_t)1;
#endif

    for (idx_t k = 1; k <= n; ++k) {
        seq_t a_coeff = pow3[k - 1];   /* 3^(k-1) */
#ifndef __cplusplus
        dp_new[0] = dp_prev[0];
        for (idx_t s = 1; s <= n; ++s) {
            dp_new[s] = dp_prev[s];
            if (s >= k) {
                seq_t temp = a_coeff * dp_new[s - k];
                dp_new[s] -= temp;   /* subtract a_coeff * dp_new[s-k] */
            }
        }
        /* swap pointers */
        seq_t *tmp = dp_prev;
        dp_prev = dp_new;
        dp_new = tmp;
#else
        dp_new[0] = dp_prev[0];
        for (idx_t s = 1; s <= n; ++s) {
            dp_new[s] = dp_prev[s];
            if (s >= k) {
                seq_t temp = a_coeff * dp_new[s - k];
                dp_new[s] -= temp;
            }
        }
        dp_prev.swap(dp_new);
#endif
    }

#ifndef __cplusplus
    seq_t result = dp_prev[n];
    free(dp_prev);
    free(dp_new);
    free(pow3);
#else
    seq_t result = dp_prev[n];
#endif
    return result;
}
