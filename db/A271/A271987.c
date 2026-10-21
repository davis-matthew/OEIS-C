/*
 * A271987  g_n(6) where g is the weak Goodstein function defined in A266202.
 * https://oeis.org/A271987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(0) = 6; For n ≥ 1, a(n) = b_{n+1}(a(n-1)) – 1, where b_k(m) =
 *            Σ_i digit_i(m, k) · (k+1)^i and digit_i(m, k) is the i‑th digit
 *            of m expressed in base k
 * Range:     u8 n <= 232, u16 n <= 381, u32 n <= 381, u64 n <= 381,
 *            u128 n <= 381, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)6;

    seq_t v = (seq_t)6;
    for (idx_t step = 1; step <= n; ++step) {
        seq_t base = (seq_t)(step + 1);   /* base k = step+1 */
        seq_t base_plus = base + 1;       /* k+1 */
        seq_t temp = v;
        seq_t res = 0;
        seq_t pow = 1;                   /* (k+1)^i */

        while (temp > 0) {
            seq_t digit = temp % base;
            temp /= base;
            if (digit != 0) res += digit * pow;
            pow *= base_plus;
        }

        v = res - 1;  /* subtract 1 after the base step */
    }
    return v;
}
