/*
 * A344734  a(n) is the smallest divisibility checking number for the n-th
 *          number coprime to 10.
 * https://oeis.org/A344734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m – inv10(m), where m is the n-th positive * integer
 *            coprime to 10 and inv10(m) is the modular inverse of 10 * modulo
 *            m (smallest positive integer k such that 10·k ≡ 1 (mod m)). *
 *            The function computes this value directly by brute‑force search
 *            * for the inverse, which is efficient for n ≤...
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute m = nth number coprime to 10 */
    static const idx_t rem[4] = {1, 3, 7, 9};
    idx_t block   = (n - 1) / 4;
    idx_t m       = block * 10 + rem[(n - 1) % 4];

    /* Special case: m = 1 */
    if (m == 1) return 1;

    /* Find the smallest k such that (10 % m) * k % m == 1 */
    idx_t mult   = 10 % m;
    idx_t inv10  = 1;
    for (inv10 = 1; inv10 < m; ++inv10)
        if ((mult * inv10) % m == 1)
            break;

    /* a(n) = m – inv10(m) */
    return (seq_t)(m - inv10);
}
