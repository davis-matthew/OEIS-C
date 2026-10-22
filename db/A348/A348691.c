/*
 * A348691  For any nonnegative number n with binary expansion Sum_{k >= 0}
 *          b_k * 2^k, a(n) is the imaginary part of f(n) = Sum_{k >= 0} b_k *
 *          (i^Sum_{j = 0..k-1} b_j) * (1+i)^k (where i denotes the imaginary
 *          unit); sequence A348690 gives the real part.
 * https://oeis.org/A348691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look, base
 * Formula:   a(n) is the imaginary part of Σ_{k≥0} b_k · i^{Σ_{j<k}b_j} ·
 *            (1+i)^k where b_k are bits of n
 * Range:     i8 n <= 8191, i16 n <= 8191, i32 n <= 8191, i64 n <= 8191,
 *            i128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t real = 1;   /* (1+i)^0 real part */
    seq_t imag = 0;   /* (1+i)^0 imaginary part */
    idx_t ones_before = 0;   /* number of 1‑bits processed so far */

    idx_t temp = n;
    /* iterate over the bits of n, least significant first */
    while (temp)
    {
        /* if the current bit is set, add its contribution */
        if (temp & 1)
        {
            switch (ones_before & 3)          /* rotation by 90° multiples */
            {
            case 0: sum += imag;              break;    /* i^0 */
            case 1: sum += real;              break;    /* i^1 */
            case 2: sum -= imag;              break;    /* i^2 */
            case 3: sum -= real;              break;    /* i^3 */
            }
            ++ones_before;                    /* the bit itself now contributes to higher positions */
        }

        /* advance (1+i)^k to (1+i)^(k+1) */
        seq_t new_real = real - imag;
        seq_t new_imag = real + imag;
        real = new_real;
        imag = new_imag;

        /* shift to the next bit */
        temp >>= 1;
    }

    return sum;
}
