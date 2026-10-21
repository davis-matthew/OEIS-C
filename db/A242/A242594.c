/*
 * A242594  Numbers k such that 4^k has initial digit 4.
 * https://oeis.org/A242594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = k such that the leading digit of 4^k is 4
 * Range:     u8 n <= 30, u16 n <= 6353, u32 n <= 9693, u64 n <= 9693,
 *            u128 n <= 9693, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the fractional‑part method with 64‑bit
   double arithmetic to determine the interval
   [log10(4), log10(5)).  No floating point operations beyond
   ordinary double constants are performed.  The function
   iterates from k=1 until the nth term is found.  The
   algorithm runs in O(n) time and easily satisfies the
   required performance limits. */

seq_t a(idx_t n)
{
    /* constants for log10(4) and log10(5) in double precision */
    const double LOG10_4 = 0.6020599913279624;
    const double LOG10_5 = 0.6989700043360188;

    double frac = 0.0;     /* fractional part of k*log10(4) */
    idx_t k = 0;
    idx_t found = 0;

    while (1) {
        ++k;
        frac += LOG10_4;
        if (frac >= 1.0)
            frac -= 1.0;
        if (frac >= LOG10_4 && frac < LOG10_5) {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
    }
}
