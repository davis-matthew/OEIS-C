/*
 * A349077  a(n) = 4^n * P(2*n, n), where P(n, x) is n-th Legendre polynomial.
 * https://oeis.org/A349077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^n * P(2*n, n) where P is Legendre polynomial. Here a(n)
 *            = Q_{2n}(n) with Q_m(x)=2^m P_m(x). The terms are computed by
 *            a(n) = Σ_{k=0}^{n} (-1)^k * binom(4n-2k, 2n-k) * binom(2n-k, k)
 *            * n^{2n-2k} The sum is evaluated using a multiplicative
 *            recurrence so that no factorials are explicitly fo...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   binom, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                /* a(0) = 1 */

    idx_t two_n = n + n;          /* 2*n   */
    idx_t four_n = two_n + two_n; /* 4*n   */

    /* Initial term k = 0  :  T = C(4n,2n) * n^(2n) */
    seq_t T = binom(four_n, two_n);
    T = T * pow_int((seq_t)n, two_n);   /* n^ (2n) = n^(2*n) */

    seq_t sum = T;                 /* sum starts with k = 0 */
    bool term_positive = true;     /* sign of current term */

    for (idx_t k = 0; k < n; ++k) {
        /* Compute next term T_{k+1} from T_k (magnitude only) */
        seq_t temp = T;

        /* Multiply by the numerator factors */
        temp = temp * (seq_t)(two_n - k);               /* (2n - k) */
        temp = temp * (seq_t)(two_n - 2*k);             /* (2n - 2k) */
        temp = temp * (seq_t)(two_n - 2*k - 1);         /* (2n - 2k - 1) */

        /* Divide by the denominator factors (exact division) */
        temp = temp / (seq_t)(k + 1);                   /* (k+1) */
        temp = temp / (seq_t)(four_n - 2*k);            /* (4n-2k) */
        temp = temp / (seq_t)(four_n - 2*k - 1);        /* (4n-2k-1) */
        temp = temp / (seq_t)n;                        /* n */
        temp = temp / (seq_t)n;                        /* n again */

        /* Store the magnitude of the next term */
        T = temp;
        /* Toggle the sign for the next term */
        term_positive = !term_positive;

        /* Accumulate into the running sum */
        if (term_positive)
            sum += T;
        else
            sum -= T;
    }

    return sum;
}
