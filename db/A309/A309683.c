/*
 * A309683  Number of odd parts appearing among the smallest parts of the
 *          partitions of n into 3 parts.
 * https://oeis.org/A309683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A309683: Number of odd smallest parts among the partitions of n into 3 parts.
   Formula: a(n) = Σ_{a=1..⌊n/3⌋} ((n-3a)/2 + 1) · (a mod 2)
   (i.e., count only odd a, each contributing ⌊(n-3a)/2⌋+1 triples.) */

seq_t a(idx_t n)
{
    seq_t result = 0;
    /* iterate over all possible smallest parts a */
    for (idx_t a = 1; a <= n / 3; ++a) {
        if (a & 1) {                       /* a is odd */
            /* number of (u, v) solutions for this a */
            seq_t term = ((seq_t)(n - 3 * a)) / 2 + 1;
            result += term;
        }
    }
    return result;
}
