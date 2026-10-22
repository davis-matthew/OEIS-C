/*
 * A309054  a(1) = 0; for m >= 0, a(3m) = 1; for m >= 1, a(3m-1) = 2*a(m-1),
 *          a(3m+1) = 2*a(m+1).
 * https://oeis.org/A309054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, hear
 * Formula:   a(0) = 1; a(1) = 0; for all m ≥ 0, a(3m) = 1; for all m ≥ 1,
 *            a(3m − 1) = 2 · a(m − 1); for all m ≥ 1, a(3m + 1) = 2 · a(m +
 *            1); (all indices are nonnegative) The sequence is symmetric:
 *            a(n) = a(−n) for n < 0, but this implementation only needs the
 *            non‑negative indices
 * Range:     u8 n <= 2187, u16 n <= 4374, u32 n <= 4374, u64 n <= 4374,
 *            u128 n <= 4374, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* if n is a multiple of 3, the value is always 1 */
    if (n % 3 == 0) return (seq_t)1;

    /* n ≡ 2 (mod 3) → a(n) = 2 * a((n−2)/3) */
    if (n % 3 == 2) {
        return ((seq_t)2) * a((n - 2) / 3);
    }

    /* n ≡ 1 (mod 3) and n ≥ 4 → a(n) = 2 * a((n+2)/3) */
    /* (n == 1 has already been handled above) */
    return ((seq_t)2) * a((n + 2) / 3);
}
