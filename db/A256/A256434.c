/*
 * A256434  Characteristic function of icosahedral numbers.
 * https://oeis.org/A256434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if ∃ m ≥ 0 such that * n = m*(5*m*m - 5*m + 2)/2, else
 *            a(n) = 0. * The function iterates m until the value exceeds n
 * Range:     u8 n <= 9778, u16 n <= 9778, u32 n <= 9778, u64 n <= 9778,
 *            u128 n <= 9778, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 128‑bit arithmetic is used to avoid overflow when seq_t is 64‑bit. */
    idx_t m = 0;
    while (1) {
        unsigned __int128 m128 = (unsigned __int128)m;
        unsigned __int128 term = (5 * m128 * m128 - 5 * m128 + 2);
        term = (m128 * term) / 2;
        unsigned __int128 n128 = (unsigned __int128)n;
        if (term == n128)
            return (seq_t)1;
        if (term > n128)
            break;
        ++m;
    }
    return (seq_t)0;
}
