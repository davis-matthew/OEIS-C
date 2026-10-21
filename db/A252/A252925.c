/*
 * A252925  Number of n X 3 nonnegative integer arrays with upper left 0 and
 *          lower right n+3-6 and value increasing by 0 or 1 with every step
 *          right or down.
 * https://oeis.org/A252925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/59875200)*n^12 + (1/997920)*n^11 + (31/1360800)*n^10 +
 *            (1/5040)*n^9 + (79/453600)*n^8 - (71/15120)*n^7 -
 *            (4127/1360800)*n^6 + (319/9072)*n^5 + (52061/5443200)*n^4 -
 *            (2587/6048)*n^3 + (3315209/1663200)*n^2 - (11097/3080)*n + 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 24, u64 n <= 174, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    __int128 r = 1;                           // coefficient of n^12
    r = r * (__int128)sn + 60;                // + 60*n^11
    r = r * (__int128)sn + 1364;              // + 1364*n^10
    r = r * (__int128)sn + 11880;             // + 11880*n^9
    r = r * (__int128)sn + 10428;             // + 10428*n^8
    r = r * (__int128)sn + (__int128)(-281160); // - 281160*n^7
    r = r * (__int128)sn + (__int128)(-181588); // - 181588*n^6
    r = r * (__int128)sn + 2105400;                 // + 2105400*n^5
    r = r * (__int128)sn + 572671;                  // + 572671*n^4
    r = r * (__int128)sn + (__int128)(-25611300);   // - 25611300*n^3
    r = r * (__int128)sn + 119347524;                // + 119347524*n^2
    r = r * (__int128)sn + (__int128)(-215725680);   // - 215725680*n
    r = r * (__int128)sn + 119750400;                // + 119750400
    return (seq_t)(r / (__int128)59875200);
}
