/*
 * A383994  Series expansion of the exponential generating function
 *          exp(wnp^!(x)) - 1 where wnp^!(x) = log(1+x) - x^2/(1+x).
 * https://oeis.org/A383994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * [x^n] ( exp( log(1+x) - x^2/(1+x) ) - 1 ) Recurrence
 *            (integer version): a(0) = 0 a(1) = 1 for n ≥ 2 a(n) = a(n-1) +
 *            Σ_{k=2..n} (-1)^{k+1}·(k+1)·Π_{j=1}^{k-1}(n-j)·a(n-k) The
 *            recurrence uses only integer operations and works for the EGF
 *            coefficients. The terms a(0)=0 are produced on de...
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 13, i64 n <= 21, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* auxiliary array that holds a_aux(0)=1 and a_aux(i)=a(i) for i>=1   */
    static seq_t *a_aux = NULL;
    static idx_t  aux_cap = 0;
    static idx_t  aux_size = 0;  /* number of elements currently stored */

    if (n == 0) return (seq_t)0;                /* a(0) = 0 by definition */

    /* Make sure the auxiliary array contains values up to index n */
    if (aux_size <= n) {
        idx_t new_cap = (aux_cap < 2*(n+1)) ? 2*(n+1) : aux_cap;
        seq_t *new_a = (seq_t*)realloc(a_aux, new_cap * sizeof(seq_t));
        if (!new_a) abort(); /* allocation failure */
        a_aux = new_a;
        aux_cap = new_cap;

        for (idx_t i = aux_size; i <= n; ++i) {
            if (i == 0) {
                a_aux[0] = (seq_t)1;          /* auxiliary a(0)=1 */
                continue;
            }

            /* start with the k = 1 contribution: a(n-1) */
            seq_t sum = a_aux[i-1];

            /* P accumulates Π_{j=1}^{k-1} (i-j) = (i-1)*(i-2)*…*(i-k+1)   */
            seq_t P = (seq_t)1;

            for (idx_t k = 2; k <= i; ++k) {
                /* multiply P by (i-(k-1)) to extend the product */
                P *= (seq_t)(i - (k-1));

                /* compute the term (k+1) * P * a(n-k) */
                seq_t term = (seq_t)(k+1) * P * a_aux[i-k];

                /* sign: (-1)^{k+1} is negative for even k, positive otherwise */
                if ((k & 1) == 0)
                    term = -term;            /* even k ⇒ negative sign */

                sum += term;
            }
            a_aux[i] = sum;
        }
        aux_size = n + 1;
    }

    return a_aux[n];
}
