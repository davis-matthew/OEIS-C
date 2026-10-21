/*
 * A276080  a(n) = A276075(A206296(n)).
 * https://oeis.org/A276080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k = 0..floor((n-1)/2)} (n-2*k)! * binomial(n-k-1, k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 25,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* compute factorials up to n: fact[i] = i! */
    seq_t *fact = alloc_seq_array(n + 1);
    fact[0] = 1;
    for (idx_t i = 1; i <= n; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }

    idx_t maxk = (n - 1) / 2;
    seq_t bin = 1;       /* binomial(n-1,0)  */
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term = fact[n - 2 * k] * bin;
        sum += term;

        if (k < maxk) {
            /* update binomial to C(n-(k+1)-1, k+1) */
            idx_t num1 = n - 2 * k - 1;
            idx_t num2 = n - 2 * k - 2;
            idx_t den1 = n - k - 1;
            idx_t den2 = k + 1;

            seq_t tmp = bin * (seq_t)num1;
            tmp = tmp * (seq_t)num2;
            bin  = tmp / ((seq_t)den1 * (seq_t)den2);
        }
    }

    free_seq_array(fact);
    return sum;
}
