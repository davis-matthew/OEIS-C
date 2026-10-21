/*
 * A294085  a(n) is the number of self-symmetric anonymous and neutral
 *          equivalence classes of preference profiles with 3 alternatives and
 *          n agents (IANC model).
 * https://oeis.org/A294085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*A005513(n-6) - A037240(n) Closed form used in the code:
 *            If n is odd, a(n) = (n+5)*(n+3)*(n+1)/48 If n is even, a(n) =
 *            ceil((n+4)^2*(n+2)/48)
 * Range:     u8 n <= 3, u16 n <= 37, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the A294085 sequence value for any n >= 0 */
seq_t a(idx_t n)
{
    /* Handle odd and even n separately to avoid overflows */
    if (n & 1ULL)  /* odd */
    {
        /* Numerator fits in the natural width of seq_t; division is exact */
        seq_t num = (seq_t)(n + 5) * (n + 3) * (n + 1);
        return num / (seq_t)48;
    }
    else          /* even */
    {
        /* Ceiling division: (num + 48 - 1) / 48 */
        seq_t num = (seq_t)(n + 4) * (n + 4) * (n + 2);
        return (num + (seq_t)47) / (seq_t)48;
    }
}
