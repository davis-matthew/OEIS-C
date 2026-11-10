/*
 * A372351  Odd bisection of A371094.
 * https://oeis.org/A372351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A371094(2*n-1), where A371094(k) = m*2^e + (4^e-1)/3, m =
 *            3k+1, e = v2(m)
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A371094_a(idx_t n)
{
    seq_t m = 3 * (seq_t)n + 1;
    idx_t e = 0;
    seq_t t = m;
    while ((t & 1) == 0) {
        e++;
        t >>= 1;
    }
    seq_t pow2e = ((seq_t)1) << e;
    seq_t pow4e = pow2e * pow2e;
    return m * pow2e + (pow4e - 1) / 3;
}

seq_t a(idx_t n)
{
    return A371094_a(2 * n - 1);
}
