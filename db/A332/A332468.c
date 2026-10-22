/*
 * A332468  a(n) = Sum_{k=1..n} mu(k) * floor(n/k)^n.
 * https://oeis.org/A332468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} mu(k) * floor(n/k)^n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   mu_of, mul_small, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mu_of.h"     /* mu_of(idx_t) -> int */
#include "../utils/mul_small.h" /* mul_small(seq_t, unsigned) -> seq_t */
#include "../utils/pow_seq.h"   /* pow_seq(seq_t, int) -> seq_t */

seq_t a(idx_t n)
{
    static idx_t M_cap = 0;
    static int *M = NULL;        /* M[i] = Σ_{k=1..i} mu(k) */

    /* Extend the Möbius prefix sum array if necessary */
    if (n > M_cap) {
        int *tmp = (int *)realloc(M, (size_t)(n + 1) * sizeof(int));
        if (!tmp) return 0;                      /* allocation failure is unlikely */
        M = tmp;
        if (M_cap == 0) M[0] = 0;                /* base case for prefix sum */
        for (idx_t i = M_cap + 1; i <= n; ++i) {
            M[i] = M[i - 1] + mu_of(i);
        }
        M_cap = n;
    }

    seq_t pos = 0, neg = 0;   /* positive and negative parts of the sum */

    for (idx_t l = 1; l <= n; ) {
        idx_t q = n / l;          /* value of floor(n/k) for k in [l,r] */
        idx_t r = n / q;          /* last index with the same q */
        int diff = M[r] - M[l - 1];   /* Σ_{k=l..r} mu(k) */

        if (diff != 0) {
            seq_t power = pow_seq((seq_t)q, (int)n);          /* q^n   */
            seq_t term  = mul_small(power, (unsigned int)(diff > 0 ? diff : -diff));

            if (diff > 0)
                pos += term;
            else
                neg += term;
        }
        l = r + 1;
    }

    return (neg == 0) ? pos : pos - neg;
}
