/*
 * A244196  Cumulative angle at the center of successive numbered sectors of a
 *          standard competition dartboard, with numbers traversed in order.
 * https://oeis.org/A244196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 18 * offsets[1];
   For n > 1: a(n) = a(n-1) + 18 * ((offsets[n] - offsets[n-1] + 20) mod 20).
   offsets represent the angular positions of the 20 dartboard sectors in
   clockwise rotation starting at sector 6 (offset 0). */
seq_t a(idx_t n)
{
    static const idx_t offsets[21] = {
        0,  /* unused placeholder for index 0 */
        16, /* sector 1 */
        3,  /* sector 2 */
        5,  /* sector 3 */
        18, /* sector 4 */
        14, /* sector 5 */
        0,  /* sector 6 (starts at 0) */
        7,  /* sector 7 */
        9,  /* sector 8 */
        12, /* sector 9 */
        1,  /* sector 10 */
        10, /* sector 11 */
        13, /* sector 12 */
        19, /* sector 13 */
        11, /* sector 14 */
        2,  /* sector 15 */
        8,  /* sector 16 */
        4,  /* sector 17 */
        17, /* sector 18 */
        6,  /* sector 19 */
        15  /* sector 20 */
    };

    seq_t value = 0;                     /* cumulative angle */
    idx_t  offset_prev = offsets[6];      /* start at sector 6 (offset 0) */

    for (idx_t i = 1; i <= n; ++i)
    {
        idx_t sector_num = (i - 1) % 20 + 1;   /* sector 1..20 repeating */
        idx_t offset_cur = offsets[sector_num];

        if (i == 1)
            value = (seq_t)offset_cur * 18;
        else
        {
            idx_t diff = (offset_cur + 20 - offset_prev) % 20;
            value += (seq_t)diff * 18;
        }
        offset_prev = offset_cur;
    }

    return value;
}
