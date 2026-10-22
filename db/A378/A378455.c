/*
 * A378455  Total number of coronal tilings of a width one length n straight
 *          polyiamond central frame with a specific hexiamond tile.
 * https://oeis.org/A378455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^{6, k≡n−1 (mod 2)} c_k where c = {1272, 2644,
 *            1412, 497, 460, 44, 40}
 * Range:     u8 none, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A378455. */
seq_t a(idx_t n)
{
    /* If the index is below the offset, return 0 as a safe default. */
    if (n == 0)
        return (seq_t)0U;

    /* Coefficients from the numerator of the generating function. */
    static const seq_t coeff[7] = {
        (seq_t)1272, /* k = 0  (even) */
        (seq_t)2644, /* k = 1  (odd)  */
        (seq_t)1412, /* k = 2  (even) */
        (seq_t)497,  /* k = 3  (odd)  */
        (seq_t)460,  /* k = 4  (even) */
        (seq_t)44,   /* k = 5  (odd)  */
        (seq_t)40    /* k = 6  (even) */
    };

    seq_t sum = (seq_t)0U;
    /* Determine starting k based on parity of n.
       For n odd (n-1 even) we sum even indices (k=0,2,4,6).
       For n even (n-1 odd)  we sum odd indices  (k=1,3,5). */
    idx_t start = (n & (idx_t)1U) ? 0U : 1U;

    /* Add the appropriate coefficients while staying within bounds and
       not exceeding the index n-1. */
    idx_t limit = n - 1U;
    for (idx_t k = start; k <= 6U && k <= limit; k += 2U)
        sum += coeff[k];

    return sum;
}
