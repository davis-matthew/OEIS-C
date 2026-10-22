/*
 * A368893  a(n) = Sum_{k=0..floor(n/3)} n^(n-2*k) * binomial(n-2*k,k).
 * https://oeis.org/A368893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} n^(n-2*k) * binomial(n-2*k,k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 0 */
    if (n == 0)
        return (seq_t)1;

    /* Initial values: power = n^n, binomial = C(n,0) = 1 */
    seq_t sum   = 0;
    seq_t base  = (seq_t)n;
    seq_t power = big_pow(base, (seq_t)n);
    seq_t binom = (seq_t)1;

    idx_t kmax = n / 3;                 /* floor(n/3) */

    for (idx_t k = 0; k <= kmax; ++k) {
        if (k > 0) {
            /* Update power: divide by n^2 each step */
            seq_t square = (seq_t)n * (seq_t)n;
            power = power / square;

            /* Update binomial via the recurrence:
               C_k+1 = C_k * (N-k)*(N-k-1)*(N-k-2) / ((k+1)*N*(N-1))
               where N = n - 2*k
            */
            idx_t kprev = k - 1;
            idx_t Nprev = n - 2 * kprev;      /* N for the previous step */

            seq_t num = (seq_t)(Nprev - kprev);
            num *= (seq_t)(Nprev - kprev - 1);
            num *= (seq_t)(Nprev - kprev - 2);

            seq_t den = (seq_t)k * (seq_t)Nprev * (seq_t)(Nprev - 1);

            binom = binom * num / den;
        }

        sum += power * binom;
    }

    return sum;
}
