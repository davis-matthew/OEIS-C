/*
 * A344345  Digitally balanced numbers in Gray code: numbers whose binary
 *          reflected Gray code has the same number of 0's as 1's.
 * https://oeis.org/A344345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 8788, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: sequential search for numbers whose binary reflected Gray code
   has equal numbers of 0's and 1's. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    idx_t found = 0;
    for (idx_t i = 1; ; ++i) {
        seq_t x = (seq_t)i;
        seq_t g = x ^ (x >> 1);
        seq_t t = g;
        int len = 0;
        int ones = 0;
        while (t) {
            len++;
            if (t & 1) ++ones;
            t >>= 1;
        }
        if ((len & 1) == 0 && ones == len / 2) {
            ++found;
            if (found == n) return x;
        }
    }
    return (seq_t)0; /* unreachable */
}
