/*
 * A305542  Number of chiral pairs of color loops of length n with exactly 3
 *          different colors.
 * https://oeis.org/A305542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -(k!/4)*(S2(floor((n+1)/2),k) + S2(ceil((n+1)/2),k)) +
 *            (k!/(2n))*Sum_{d|n} φ(d)*S2(n/d,k) with k=3 where S2(t,3) = (3^t
 *            - 3*2^t + 3)/6. (indices start at 1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   _factor_uint64, _s2_of_3, _sum_divisor_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/_factor_uint64.h"
#include "../utils/_s2_of_3.h"
#include "../utils/_sum_divisor_rec.h"

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* pre‑compute powers of 2 and 3 up to n */
    seq_t *pow2 = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *pow3 = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    pow2[0] = pow3[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
    {
        pow2[i] = pow2[i - 1] * (seq_t)2;
        pow3[i] = pow3[i - 1] * (seq_t)3;
    }

    /* S2(floor((n+1)/2),3) + S2(ceil((n+1)/2),3) */
    idx_t m1 = (n + 1) / 2;    /* floor((n+1)/2) */
    idx_t m2 = (n + 2) / 2;    /* ceil((n+1)/2) */
    seq_t s2_m1 = _s2_of_3(m1, pow2, pow3);
    seq_t s2_m2 = _s2_of_3(m2, pow2, pow3);
    seq_t s2_sum = s2_m1 + s2_m2;

    /* factorise n */
    uint64_t pr[32];
    int     ex[32];
    size_t  pcnt;
    _factor_uint64((uint64_t)n, pr, ex, &pcnt);

    /* sum over divisors φ(d)*S2(n/d,3) using 128‑bit accumulator */
    unsigned __int128 sum128 = 0;
    _sum_divisor_rec(0, pr, ex, pcnt, (seq_t)1, (idx_t)1,
                     n, pow2, pow3, &sum128);

    /* compute a(n) with 128‑bit arithmetic to avoid overflow in intermediate steps */
    unsigned __int128 term_b   = (unsigned __int128)6 * sum128;
    unsigned __int128 term_a   = (unsigned __int128)3 * (unsigned __int128)n * (unsigned __int128)s2_sum;
    unsigned __int128 numerator = term_b - term_a;
    unsigned __int128 denominator = (unsigned __int128)2 * (unsigned __int128)n;
    unsigned __int128 result128 = numerator / denominator;

    seq_t result = (seq_t)result128;

    free(pow2);
    free(pow3);
    return result;
}
