/*
 * A400137  Triangle T(n, k) read by rows: the n-th row gives the greatest
 *          common infinitary divisor of n and k, n >= 1, 1 < = k <= n.
 * https://oeis.org/A400137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the flattened triangle of * T(n,k) = ∏_{p^e‖n}
 *            p^{bitand(e, v_p(k))} (OEIS A400137)
 * Range:     u8 n <= 8757, u16 n <= 8757, u32 n <= 8757, u64 n <= 8757,
 *            u128 n <= 8757, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find the row r and column k from the flattened index n */
    idx_t r   = 1;
    idx_t cum = 1;                /* cumulative count up to row r */
    while (cum < n) {
        ++r;
        cum += r;
    }
    idx_t prev = cum - r;         /* terms before row r */
    idx_t k    = n - prev;        /* column within row r (1‑based) */

    if (r <= 1) return 1;         /* row 1 contains only 1 */

    seq_t result = 1;
    idx_t temp_r = r;
    idx_t kk = k;                 /* copy of k for repeated use */

    /* factorise r by trial division */
    for (idx_t p = 2; p * p <= temp_r; ++p) {
        if (temp_r % p == 0) {
            idx_t e = 0;
            while (temp_r % p == 0) {
                ++e;
                temp_r /= p;
            }

            /* compute v_p(k) */
            idx_t tmp_k = kk;
            idx_t v = 0;
            while (tmp_k % p == 0) {
                ++v;
                tmp_k /= p;
            }

            idx_t bit = e & v;    /* bitwise AND of exponents */
            for (idx_t j = 0; j < bit; ++j) {
                result *= (seq_t)p;
            }
        }
    }

    /* remaining prime factor, if any */
    if (temp_r > 1) {
        idx_t p = temp_r;
        idx_t e = 1;
        idx_t tmp_k = kk;
        idx_t v = 0;
        while (tmp_k % p == 0) {
            ++v;
            tmp_k /= p;
        }
        idx_t bit = e & v;
        for (idx_t j = 0; j < bit; ++j) {
            result *= (seq_t)p;
        }
    }

    return result;
}
