/*
 * A289642  Number of 2-digit numbers whose digits add up to n.
 * https://oeis.org/A289642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   a(n) = min(n, 19 - n) for 1 ≤ n ≤ 18; otherwise 0
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > 18)
        return 0;
    seq_t n_seq = (seq_t)n;
    seq_t val2   = (seq_t)19 - n_seq;
    return (n_seq < val2) ? n_seq : val2;
}
