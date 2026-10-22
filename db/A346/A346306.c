/*
 * A346306  Position in A076478 of the binary complement of the n-th word in
 *          A076478.
 * https://oeis.org/A346306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, base
 * Formula:   a(n) = 3*(2^d-1) - n, where d = ⌊log₂(n+1)⌋
 * Range:     u8 n <= 254, u16 n <= 9949, u32 n <= 9949, u64 n <= 9949,
 *            u128 n <= 9949, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute  a(n)   for n ≥ 1
 *   a(n) = 3*(2^d-1) - n
 *   d = floor_log2(n+1)  (the largest d with 2^d ≤ n+1)
 */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset is 1, but guard for safety */

    /* Find d = floor(log2(n+1)) */
    idx_t n1 = n + 1;              /* n+1 > 0 because n ≥ 1 */
    int d;

    /* Prefer compiler intrinsic when available */
#if defined(__GNUC__) || defined(__clang__)
    /* __builtin_clzll is undefined for 0; n1 is never 0 here */
    d = 63 - __builtin_clzll(n1);
#else
    /* Portable fallback: shift until the value exceeds n1 */
    d = 0;
    idx_t val = 1;
    while (val <= n1 / 2) {       /* ensure no overflow */
        val <<= 1;
        ++d;
    }
#endif

    /* Compute 2^d as a seq_t value */
    seq_t pow2d = (seq_t)1;
    for (int i = 0; i < d; ++i)
        pow2d <<= 1;                /* safe: d ≤ 63 for 64‑bit n */

    /* a(n) = 3 * (pow2d - 1) - n */
    seq_t result = (seq_t)3 * (pow2d - 1) - (seq_t)n;
    return result;
}
