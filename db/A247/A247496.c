/*
 * A247496  a(n) = n!*[x^n](exp(n*x)*BesselI_{1}(2*x)/x), n>=0, main diagonal
 *          of A247495.
 * https://oeis.org/A247496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=0..⌊n/2⌋} ((j+1) * n^(n-2j) * n!) / ((j+1)!^2 *
 *            (n-2j)!)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *pow_arr = NULL;
    static size_t pow_cap = 0;

    /* Ensure we have room for powers up to n */
    if (pow_cap <= n) {
        seq_t *new_arr = (seq_t*)realloc(pow_arr, sizeof(seq_t)*(n+1));
        if (!new_arr) { /* allocation failure: return 0 as a safe fallback */
            return (seq_t)0;
        }
        pow_arr = new_arr;
        pow_cap = n+1;
    }

    /* Compute powers of n: pow_arr[k] = n^k  (k = 0..n) */
    pow_arr[0] = (seq_t)1;
    seq_t n_seq = (seq_t)n;
    for (idx_t k = 1; k <= n; ++k)
        pow_arr[k] = pow_arr[k-1] * n_seq;

    seq_t sum = (seq_t)0;
    seq_t A = (seq_t)1;          /* product of 2*j descending terms */
    seq_t f_jp = (seq_t)1;       /* (j+1)!  (starts with j=0 => 1!) */

    idx_t j_max = n / 2;
    for (idx_t j = 0; j <= j_max; ++j) {
        seq_t power = pow_arr[n - 2*j];
        seq_t denom = f_jp * f_jp;                /* (j+1)!^2 */
        seq_t term = (seq_t)(j + 1) * power * A;
        term = term / denom;                      /* exact division */
        sum += term;

        if (j < j_max) {                         /* prepare for next j */
            /* A *= (n-2*j) * (n-2*j-1)  -> product of two new factors */
            A *= (n_seq - (seq_t)(2*j));
            A *= (n_seq - (seq_t)(2*j + 1));
            /* f_jp *= (j+2)  -> (j+2)! for next iteration */
            f_jp *= (seq_t)(j + 2);
        }
    }

    return sum;
}
