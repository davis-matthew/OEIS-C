/*
 * A349598  E.g.f. satisfies: log(A(x)) = exp(x*A(x)^2) - 1.
 * https://oeis.org/A349598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (2*n+1)^(k-1) * Stirling2(n,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n)
{
    static seq_t *S      = NULL; /* current Stirling row S[n][k] stored as a 1‑D array
                                   S[k] holds S[n][k] after finishing the row for n */
    static seq_t *a_vals = NULL; /* cached sequence values */
    static idx_t  computed = 0;  /* highest n computed so far (>=0) */
    static size_t S_cap    = 0;  /* capacity of S array */
    static size_t a_cap    = 0;  /* capacity of a_vals array */

    /* first call: initialise for n = 0 */
    if (S == NULL) {
        grow_seq_array(&S, &S_cap, 1);      /* S[0] = 1  (S(0,0)) */
        S[0] = 1;

        grow_seq_array(&a_vals, &a_cap, 1); /* a(0) = 1 */
        a_vals[0] = 1;
        computed = 0;
    }

    /* compute rows up to requested n (if needed) */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* ensure arrays large enough for new row i */
        if (i + 1 > S_cap)   grow_seq_array(&S,   &S_cap, i + 1);
        if (i + 1 > a_cap)   grow_seq_array(&a_vals, &a_cap, i + 1);

        /* start building row i from row i‑1 in place.
           S[0] corresponds to k = 0 of the previous row,
           S[1..i] correspond to k = 1..i of the new row. */
        S[i] = 0; /* new element for k = i */

        /* use recurrence S(n,k) = k * S(n-1,k) + S(n-1,k-1)
           traverse k descending so that S[k-1] still holds S(n-1,k-1) */
        for (idx_t k = i; k >= 1; --k) {
            S[k] = (seq_t)k * S[k] + S[k - 1];
        }

        /* After the recurrence S[0] must be set to 0 for n > 0 */
        S[0] = 0;

        /* evaluate the sum for a(i) */
        seq_t sum   = 0;
        seq_t base  = (seq_t)(2 * i + 1);
        seq_t power = 1; /* (2*i+1)^(k-1), starts with k=1 */

        for (idx_t k = 1; k <= i; ++k) {
            sum += power * S[k];
            power *= base;
        }

        a_vals[i] = sum;
        computed  = i;
    }

    return a_vals[n];
}
