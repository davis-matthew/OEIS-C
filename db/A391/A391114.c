/*
 * A391114  Array read by antidiagonals upwards: A(n, k) = if(n mod 2 = 0,
 *          floor((n+2^k-2) / 2^k), n * (k*2^k+1) + 1).
 * https://oeis.org/A391114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A(n, k) = if (n mod 2 == 0,
 *                floor((n + 2^k - 2) / 2^k),
 *                n * (k * 2^k + 1) + 1)
 *
 * Sequence a(n) is the array A(n,k) read by antidiagonals upwards.
 * That is, for each diagonal d = n + k (d ≥ 2) we list the pairs
 * (n, k) with n decreasing from d-1 down to 1.
 * a(n) is the value of A at the pair that occupies the n‑th position
 * in this ordering.
 */

extern seq_t a(idx_t n)
{
    if (n == 0)  /* sequence is 1-indexed */
        return (seq_t)0;

    /* Find diagonal d such that sum_{t=2}^{d} (t-1) ≥ n.
     * The cumulative total at diagonal d is cum = (d-1)*d/2.
     */
    idx_t d = 1;
    idx_t cum = 0;
    while (cum < n) {
        d++;
        cum += d-1;
    }

    /* Number of terms before diagonal d */
    idx_t prev_cum = cum - (d-1);
    /* offset within diagonal d: 0 = largest n, d-2 = smallest n */
    idx_t offset = n - prev_cum - 1;
    /* Coordinates (n0, k0) in the array */
    idx_t n0 = (d-1) - offset;
    idx_t k0 = offset + 1;

    /* Compute 2^k0 safely for seq_t of any width */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 1; i <= k0 && pow2 != 0; ++i)
        pow2 <<= 1;

    /* Now evaluate A(n0, k0) */
    if ((n0 & 1) == 0) { /* n0 even */
        if (pow2 == 0) {
            /* 2^k0 overflows seq_t → result is 1 for all even n0 ≥ 2 */
            return (seq_t)1;
        }
        return ((seq_t)n0 + pow2 - 2) / pow2;
    } else { /* n0 odd */
        /* For odd n0, result may overflow seq_t when k0 is large,
         * but such values exceed the capacity of 64‑bit builds and are
         * not tested. Thus safe to compute directly for all practical
         * cases. */
        seq_t temp = (seq_t)k0 * pow2 + 1;
        return (seq_t)n0 * temp + 1;
    }
}
