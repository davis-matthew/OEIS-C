/*
 * A348138  For any number n with factorial base representation (d_1, ...,
 *          d_k), for i = 1..k, let f_i be the number of i's in (d_1, ...,
 *          d_k); the factorial base representation of a(n) is (f_1, ...,
 *          f_k).
 * https://oeis.org/A348138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n if n = 0 or a factorial number. Otherwise, write n in
 *            factorial base (d_1,…,d_k), count f_i = number of i’s in
 *            (d_1,…,d_k) for 1 ≤ i ≤ k, and set a(n) to have factorial‑base
 *            digits (f_k,…,f_1)
 * Range:     u8 n <= 122, u16 n <= 5040, u32 n <= 5040, u64 n <= 5040,
 *            u128 n <= 5040, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U) return 0;

    /* factorial‑base digits of n (least significant first) */
    int digits[64];
    int k = 0;
    idx_t t = n;
    while (t > 0U) {
        idx_t divisor = (idx_t)(k + 2);        /* divisor = (position+1) */
        digits[k] = (int)(t % divisor);       /* digit d_{k+1} */
        t /= divisor;
        ++k;
    }

    /* frequency of each digit value */
    int f[64];
    for (int i = 0; i <= k; ++i) f[i] = 0;
    for (int i = 0; i < k; ++i) {
        int v = digits[i];
        if (v > 0) f[v]++;                    /* count only i ≥ 1 */
    }

    /* reconstruct a(n) from reversed frequencies */
    seq_t result = 0;
    seq_t fact   = 1;
    for (int j = 1; j <= k; ++j) {
        fact = fact * (seq_t)j;                /* j! */
        result = result + (seq_t)f[k - j + 1] * fact;
    }

    return result;
}
