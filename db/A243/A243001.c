/*
 * A243001  a(n) = date of Easter in March of n, in the Gregorian calendar
 *          after A.D. 1582. a(n) = 0 if Easter occurs in April.
 * https://oeis.org/A243001
 *
 * Offset:    1583    (a(1583) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Gregorian Easter date. a(n) returns the day of Easter in March
 *            for year n (Gregorian calendar), or 0 if Easter falls in April.
 *            For the sequence, index n starts at 1583
 * Range:     u8 none, u16 n <= 1664, u32 n <= 1664, u64 n <= 1664,
 *            u128 n <= 1664, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1583) return 0;          /* before the offset */
    seq_t Y = (seq_t)n;

    seq_t A = Y % 19;
    seq_t B = Y / 100;
    seq_t C = Y % 100;
    seq_t D = B / 4;
    seq_t E = B % 4;
    seq_t F = (B + 8) / 25;          /* unused but kept for clarity */
    seq_t G = (B - F + 1) / 3;
    seq_t H = (19 * A + B - D - G + 15) % 30;
    seq_t I = C / 4;
    seq_t K = C % 4;
    seq_t L = (32 + 2 * E + 2 * I - H - K) % 7;
    seq_t M = (A + 11 * H + 22 * L) / 451;

    seq_t month = (H + L - 7 * M + 114) / 31;                /* 3 = March, 4 = April */
    seq_t day   = (H + L - 7 * M + 114) % 31 + 1;

    return (month == 3) ? day : 0;
}
