/*
 * A260918  Number of squares of all sizes in polyominoes obtained by union of
 *          two pyramidal figures (A092498) with intersection equals A002623.
 * https://oeis.org/A260918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            (52*n^3+90*n^2+20*n-3*(32*floor((n+1)/3)+3*(1-(-1)^n)))/144
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 461, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;
    seq_t n3 = n2 * sn;

    seq_t part1 = ((seq_t)52) * n3;
    seq_t part2 = ((seq_t)90) * n2;
    seq_t part3 = ((seq_t)20) * sn;
    seq_t sum = part1 + part2 + part3;

    seq_t floor_val = (sn + ((seq_t)1)) / ((seq_t)3);
    seq_t odd_term = (sn & ((seq_t)1)) ? ((seq_t)2) : ((seq_t)0);
    seq_t sub = ((seq_t)3) * (((seq_t)32) * floor_val + ((seq_t)3) * odd_term);

    seq_t num = sum - sub;
    return num / ((seq_t)144);
}
