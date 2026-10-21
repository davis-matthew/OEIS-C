/*
 * A295514  a(n) = 2^bil(n) - bil(n) where bil(0) = 0 and bil(n) =
 *          floor(log_2(n)) + 1 for n > 0.
 * https://oeis.org/A295514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(floor(log2(n))+1) - (floor(log2(n))+1), a(0)=1 (where
 *            floor(log2(n))+1 is the bit‑length of n. For n=0 the bit‑length
 *            is defined to be 0.)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the bit‑length k = floor(log2(n)) + 1, i.e. the number of
       bits needed to represent n.  For n==0, k stays 0. */
    idx_t k = 0;
    idx_t tmp = n;
    while (tmp) {
        ++k;
        tmp >>= 1;
    }

    /* 2^k can be obtained by shifting 1 left k positions.
       This works for any seq_t implementation (including arbitrary-
       precision types). */
    seq_t value = ((seq_t)1 << k) - (seq_t)k;
    return value;
}
