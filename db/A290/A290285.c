/*
 * A290285  Determinant of circulant matrix of order 3 with entries in the
 *          first row (-1)^j * Sum_{k>=0} binomial(n,3*k+j), j=0,1,2.
 * https://oeis.org/A290285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (s0 + s2 – s1) * (s0² + s1² + s2² + s0·s1 + s1·s2 –
 *            s0·s2) where s_j = (2ⁿ + 2·cos(π·(n – 2j)/3)) / 3 , j = 0,1,2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine computes the OEIS sequence A290285 by evaluating the
   determinant of the circulant matrix with first‑row entries
   c_j = (–1)^j · Σ_k binomial(n, 3k + j).  The binomial sums are
   expressed through a simple modular‑6 formula using powers of 2 and
   the six possible values of 2·cos(π·m/3).  All arithmetic uses the
   generic integer type seq_t. */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* 2ⁿ as a seq_t (big integer if needed) */
    seq_t pow2 = seq_pow((seq_t)2, n);

    /* 2·cos(π·m/3) for m = 0..5  */
    static const int8_t cos2[6] = { 2,  1, -1, -2, -1,  1 };

    /* compute s0,s1,s2 */
    seq_t s[3];
    idx_t n_mod6 = n % 6;          /* 0..5 */

    for (int j = 0; j < 3; ++j) {
        idx_t shift = (2 * j) % 6;                       /* 0,2,4 */
        int m = (int)(n_mod6 - shift);
        m %= 6;
        if (m < 0) m += 6;                               /* 0..5 */
        int8_t c2 = cos2[m];                             /* 2·cos value */

        seq_t num;
        if (c2 >= 0)
            num = pow2 + (seq_t)c2;
        else
            num = pow2 - (seq_t)(-c2);

        s[j] = num / (seq_t)3;                            /* division exact */
    }

    /* λ0 = s0 + s2 – s1  (guaranteed non‑negative) */
    seq_t lambda0 = s[0] + s[2];
    lambda0 = lambda0 - s[1];

    /* v = s0² + s1² + s2² + s0·s1 + s1·s2 – s0·s2 */
    seq_t v = s[0]*s[0] + s[1]*s[1] + s[2]*s[2]
            + s[0]*s[1] + s[1]*s[2];
    v = v - s[0]*s[2];

    /* determinant = λ0 · v  */
    return lambda0 * v;
}
