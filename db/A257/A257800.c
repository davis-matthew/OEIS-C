/*
 * A257800  Sequence A233271 reduced modulo 2: a(n) = A000035(A233271(n)); the
 *          parity of each term in the infinite trunk of inverted binary
 *          beanstalk.
 * https://oeis.org/A257800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A233271(n) mod 2, where A233271(0)=0, A233271(k+1) =
 *            A233271(k) + 1 + (number of 0's in binary expansion of
 *            A233271(k))
 * Range:     u8 n <= 73, u16 n <= 8727, u32 n <= 8727, u64 n <= 8727,
 *            u128 n <= 8727, big any n
 * Validated: standard, pass, 140 terms through n = 8483 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A233271_val(idx_t n)
{
    seq_t x = 0;
    for (idx_t i = 0; i < n; ++i)
    {
        seq_t t = x;
        seq_t zeros = 0;
        while (t > 0)
        {
            if ((t & 1) == 0)
                zeros++;
            t >>= 1;
        }
        x = x + 1 + zeros;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t v = A233271_val(n);
    return (seq_t)(v & 1);
}
