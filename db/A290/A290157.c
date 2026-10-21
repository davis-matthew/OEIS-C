/*
 * A290157  Tribonacci-like sequence of composite numbers with a(0) =
 *          151646890045, a(1) = 836564809606 and a(2) = 942785024683.
 * https://oeis.org/A290157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 151646890045, a(1) = 836564809606, a(2) = 942785024683,
 *            a(n) = a(n-1) + a(n-2) + a(n-3) for n > 2
 * Range:     u8 none, u16 none, u32 none, u64 n <= 29, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base terms (constants are 64‑bit, safely promoted to seq_t). */
    static const seq_t T0 = (seq_t)151646890045ULL;
    static const seq_t T1 = (seq_t)836564809606ULL;
    static const seq_t T2 = (seq_t)942785024683ULL;

    if (n == 0) return T0;
    if (n == 1) return T1;
    if (n == 2) return T2;

    seq_t a0 = T0, a1 = T1, a2 = T2, next;
    /* Iteratively compute terms; idx_t is unsigned, i <= n works for all n>=3. */
    for (idx_t i = 3; i <= n; ++i) {
        next = a0 + a1 + a2;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }

    return a2;
}
