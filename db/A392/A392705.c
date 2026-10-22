/*
 * A392705  Negated least odd-indexed part in row n of A391983 (the 0-based
 *          partial alternating sums of the n-th composition in standard
 *          order).
 * https://oeis.org/A392705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = negated least odd-indexed part in the 0-based partial
 *            alternating sums of the n-th composition in standard order
 *            (graded reverse-lexicographic). a(1) = 0, a(2) = 0, a(3) = 0,
 *            ... as given
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert to the k used by the bijection:
       k = 2 * n  (1‑based n -> 0‑based k = 2n). */
    idx_t k = n << 1;          /* k = 2*n; shift is safe for n up to 2^63-1   */
    idx_t temp = k;            /* copy for bit extraction */
    int pos = 0;               /* current bit position (LSB = 0)   */
    int m = 0;                 /* number of parts in the composition   */
    seq_t diff[64];            /* differences of consecutive positions  */
    idx_t lastpos = 0;         /* previous 1‑bit position, start at 0   */

    /* Extract the set‑bit positions from least significant to most, creating the differences. */
    while (temp) {
        if (temp & 1ULL) {
            diff[m++] = (seq_t)(pos - lastpos);  /* part size */
            lastpos = pos;
        }
        ++pos;
        temp >>= 1;
    }

    /* Compute 0‑based partial alternating sums using the recurrence
       S_j = -S_{j-1} + q_j, where q_j is the j‑th part of the composition.
       We need the minimum over odd‑indexed parts (1‑based), i.e. over even j (0‑based). */
    long long S = 0;          /* current partial alternating sum   */
    long long min_even = 0;   /* current minimum over even j      */

    for (int i = 0; i < m; ++i) {
        long long q = (long long)diff[m - 1 - i]; /* reverse diff to get parts in order   */
        S = -S + q;                              /* recurrence                                      */
        if (((i + 1) & 1) == 0) {                /* j = i+1 is even (1‑based odd) */
            if (S < min_even) min_even = S;
        }
    }

    /* Negate the minimum (which is <= 0) to obtain the non‑negative result. */
    long long result = -min_even;
    return (seq_t)result;
}
