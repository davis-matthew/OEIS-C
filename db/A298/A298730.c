/*
 * A298730  Expansion of Product_{k>=1} 1/(1 - x^(k*(k+1)/2))^(k*(k+1)/2).
 * https://oeis.org/A298730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 23, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A298730: expansion of ∏_{k≥1} 1/(1−x^{k(k+1)/2})^{k(k+1)/2}
    Algorithm:
    – For each triangular number t = k(k+1)/2 ≤ n compute r_t = t².
    – For all multiples m of t up to n add t² to r[m].
    – Non‑recursive recurrence: a(0)=1,
      a(n)= (1/n) * Σ_{k=1}^{n} r_k * a(n−k).
      (This follows from taking logarithms and exponentials of the
       generating function.)
*/

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t N = n;
    /* allocate arrays for r_k and a_k */
    seq_t *r = (seq_t*)malloc((size_t)(N + 1) * sizeof(seq_t));
    seq_t *a_arr = (seq_t*)malloc((size_t)(N + 1) * sizeof(seq_t));
    if (!r || !a_arr) {
        if (r) free(r);
        if (a_arr) free(a_arr);
        return (seq_t)0;  /* allocation failed */
    }

    /* initialise to zero */
    for (idx_t i = 0; i <= N; ++i) {
        r[i] = (seq_t)0;
        a_arr[i] = (seq_t)0;
    }

    /* compute r_k = Σ_{t|k , t triangular} t²   (t = m(m+1)/2) */
    for (idx_t m = 1; ; ++m) {
        idx_t t = m*(m + 1)/2;   /* triangular number */
        if (t > N) break;
        seq_t t2 = (seq_t)t * (seq_t)t;
        for (idx_t k = t; k <= N; k += t) {
            r[k] += t2;
        }
    }

    /* dynamic programming for a(n) */
    a_arr[0] = (seq_t)1;
    for (idx_t i = 1; i <= N; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            sum += r[k] * a_arr[i - k];
        }
        a_arr[i] = sum / (seq_t)i;   /* division is exact */
    }

    seq_t result = a_arr[N];
    free(r);
    free(a_arr);
    return result;
}
