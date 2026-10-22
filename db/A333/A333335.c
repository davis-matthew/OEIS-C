/*
 * A333335  a(n) is the smallest positive number k such that n divides 4^k +
 *          k.
 * https://oeis.org/A333335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 8739, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = smallest positive k such that n divides 4^k + k */
seq_t a(idx_t n)
{
    seq_t pow4 = ((seq_t)4) % (seq_t)n;
    seq_t k = 1;
    while (1) {
        if (((pow4 + k) % (seq_t)n) == 0)
            return k;
        ++k;
        pow4 = (pow4 * ((seq_t)4)) % (seq_t)n;
    }
}
