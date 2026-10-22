/*
 * A387125  Coefficient of x^n in the expansion of ((1+x) * (1+x^4))^n.
 * https://oeis.org/A387125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} binomial(n,k) * binomial(n,4*k)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 43, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for the type definitions that the build supplies */

seq_t a(idx_t n)
{
    /* The sum runs over k = 0..floor(n/4).  We compute the necessary binomials
       incrementally to keep the runtime linear in n. */
    seq_t sum = 0;
    seq_t Cnk = 1;        /* binomial(n, k)      */
    seq_t Cn4k = 1;       /* binomial(n, 4*k)    */
    idx_t  r   = 0;       /* current value of 4*k */

    idx_t maxk = n / 4;   /* floor(n/4) */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        sum += Cnk * Cn4k;

        if (k == maxk)
            break;  /* finished */

        /* Update Cnk for k+1: C(n,k+1) = C(n,k) * (n-k)/(k+1) */
        Cnk = Cnk * ((seq_t)(n - k)) / ((seq_t)(k + 1));

        /* Advance Cn4k from value at r=4*k to r=4*(k+1) by four steps. */
        for (int step = 0; step < 4; ++step)
        {
            /* Current C(n,r), next C(n,r+1) = C(n,r) * (n-r)/(r+1) */
            ++r;
            Cn4k = Cn4k * ((seq_t)(n - r + 1)) / ((seq_t)r);
        }
    }

    return sum;
}
