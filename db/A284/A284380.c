/*
 * A284380  Numbers k with digits 5 and 7 only.
 * https://oeis.org/A284380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 1022, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumeration of numbers whose decimal digits are only 5 or 7.
   For n ≥ 1: let L be the unique integer with 2^(L+1) ≥ n+2 > 2^L.
   Then a(n) is obtained by writing the offset
       m = n - (2^L - 2) - 1   (0 ≤ m < 2^L)
   in binary with exactly L bits, and replacing each bit
   0→5 and 1→7 to form a decimal number.  This yields,
   for n = 1…: 5, 7, 55, 57, 75, 77, 555, 557, …  */

seq_t a(idx_t n)
{
    /* undefined offset – return 0, the same as earlier approach */
    if (n == 0)
        return (seq_t)0;

    /* find smallest L such that 2^(L+1) >= n+2          */
    idx_t t = n + 2;                /* target bound                    */
    idx_t pow = 1;                  /* 2^i                            */
    idx_t i = 0;
    while (pow < t) {
        pow <<= 1;                  /* grow by factor of 2            */
        ++i;
    }
    /* now pow = 2^i >= n+2, so L = i-1                      */
    idx_t L = i - 1;

    /* cumulative count of terms up to length L-1:
       cum_before = 2^L - 2                                */
    idx_t pow2L = (idx_t)1 << L;   /* 2^L, safe because L <= 63 for n within limits */
    idx_t cum_before = pow2L - 2;

    /* zero‑based offset within length L */
    idx_t offset = n - cum_before - 1;   /* 0 … 2^L-1 */

    /* construct the decimal number from the bits of offset   */
    seq_t result = (seq_t)0;
    for (idx_t bitpos = L; bitpos > 0; --bitpos) {
        idx_t bit = (offset >> (bitpos - 1)) & 1;
        result = result * 10 + (bit ? (seq_t)7 : (seq_t)5);
    }
    return result;
}
