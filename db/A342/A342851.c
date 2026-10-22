/*
 * A342851  Remove duplicates in the decimal digit-reversal of n.
 * https://oeis.org/A342851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 231, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A342851: numbers that are not multiples of 10 (0 included).
   a(1) = 0, a(n) = n-1 + floor((n-2)/9) for n > 1. */
seq_t a(idx_t n)
{
    /* n is 1‑based.  k = n-1 is the zero‑based index. */
    idx_t k = n - 1;
    if (k == 0)      /* a(1) == 0 */
        return 0;

    /* Every 9 numbers we skip one multiple of 10.  The mapping from
       the zero‑based index k to the value m is:
          let q = floor(k/9), r = k % 9
          if r == 0: r = 9, q = q-1
          m = 10*q + r
    */
    idx_t q = k / 9;
    idx_t r = k % 9;
    if (r == 0) {
        r = 9;
        --q;
    }
    return (seq_t)10 * q + r;
}
