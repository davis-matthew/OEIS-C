/*
 * A338731  Generating function Sum_{n >= 0} a(n)*x^n = Sum_{k>=1}
 *          x^(k*(3*k+1)/2)/(1-x^k).
 * https://oeis.org/A338731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the number of k ≥ 1 such that n - k(3k+1)/2 is *
 *            divisible by k and non‑negative. Equivalently, a(n) is the *
 *            coefficient of xⁿ in * Σ_{k≥1} x^{k(3k+1)/2}/(1−x^k)
 * Range:     u8 n <= 346, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t n_val = (seq_t)n;
    for (idx_t k = 1; ; ++k) {
        seq_t sk = (seq_t)k;
        seq_t t_k = sk * ((seq_t)3 * sk + 1) / 2;  /* k*(3k+1)/2 */
        if (t_k > n_val)
            break;
        seq_t rem = n_val - t_k;                  /* remaining part */
        if (rem % sk == 0)
            ++count;
    }
    return count;
}
