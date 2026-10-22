/*
 * A390085  Half the length of the longest irreducible zero-sum subsets of
 *          {-2*n+1, -2*n+3, ..., -3, -1, 1, 3, ..., 2*n-3, 2*n-1}.
 * https://oeis.org/A390085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n)=floor(sqrt(2*n+4)-1); a(3)=1 (special case)
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3)
        return (seq_t)1;
    idx_t val = 2 * n + 4;
    idx_t sq  = 0;
    while ((sq + 1) * (sq + 1) <= val)
        ++sq;
    return (seq_t)(sq - 1);
}
