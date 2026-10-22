/*
 * A381774  Expansion of ( (1/x) * Series_Reversion( x/((1+x) * C(x))^4 )
 *          )^(1/4), where C(x) is the g.f. of A000108.
 * https://oeis.org/A381774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(4*n+2*k+1, k) * binomial(4*n+1,
 *            n-k) / (4*n+2*k+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* binomial(idx_t a, idx_t b) */
#include "../utils/gcd.h"        /* gcd(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    /* Pre‑compute constants that appear repeatedly. */
    idx_t four_n = 4 * n;
    idx_t four_n_plus1 = four_n + 1;       /* 4*n + 1 */

    /* B(k) = C(4*n+2*k+1 , k)  with B(0)=1                      */
    seq_t B = 1;

    /* S(k) = C(4*n+1 , n-k)  with S(0)=C(4*n+1 , n)             */
    seq_t S = binomial(four_n_plus1, n);

    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        /* Denominator for the current term: D = 4*n + 2*k + 1 */
        seq_t D = (seq_t)(four_n + 2 * k + 1);

        /* Compute the term using cancellation to avoid overflow. */
        seq_t g1 = gcd(B, D);
        seq_t B_red = B / g1;
        seq_t d    = D / g1;

        seq_t g2 = gcd(S, d);
        seq_t S_red = S / g2;
        d = d / g2;                         /* Should be 1 */

        sum += B_red * S_red;

        if (k == n)
            break;                           /* No more updates needed */

        /* Update B for next k:  B_{k+1} = B_k * (4n+2k+3)(4n+2k+2) / ((k+1)(4n+k+2)) */
        seq_t f1 = (seq_t)(four_n + 2 * k + 3);
        seq_t f2 = (seq_t)(four_n + 2 * k + 2);
        seq_t denomB = ((seq_t)(k + 1)) * ((seq_t)(four_n + k + 2));

        seq_t g3 = gcd(B, denomB); B = B / g3; denomB = denomB / g3;
        seq_t g4 = gcd(f1, denomB); f1 = f1 / g4; denomB = denomB / g4;
        seq_t g5 = gcd(f2, denomB); f2 = f2 / g5; denomB = denomB / g5;
        /* denomB is now 1 */
        B = B * f1 * f2;

        /* Update S for next k:  S_{k+1} = S_k * (n-k) / (3n+k+2) */
        seq_t numS = (seq_t)(n) - k;
        seq_t denomS = (seq_t)(3 * n + k + 2);

        seq_t gs1 = gcd(S, denomS); S = S / gs1; denomS = denomS / gs1;
        seq_t gs2 = gcd(numS, denomS); numS = numS / gs2; denomS = denomS / gs2;
        /* denomS is now 1 */
        S = S * numS;
    }

    return sum;
}
