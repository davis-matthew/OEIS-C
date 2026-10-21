/*
 * A242892  Beginning with a centrally symmetric 'Star' configuration of 9
 *          rhombi with rotational symmetry, number of tiles that can be added
 *          to the free edges of the tiling.
 * https://oeis.org/A242892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A242892 – number of tiles that can be added to a
 * 9‑rhombus star configuration.
 *
 * The values are periodic with period 6:
 *   a(n) = 9   if n mod 6 ∈ {1,2}
 *   a(n) = 18  if n mod 6 ∈ {0,3}
 *   a(n) = 27  if n mod 6 ∈ {4,5}
 */

seq_t a(idx_t n)
{
    /* table[ r ] gives a(n) where r = n % 6 */
    static const seq_t table[6] = {18, 9, 9, 18, 27, 27};
    return table[n % 6];
}
