/*
 * A278828  King's moves in chess: possible difference between origin and
 *          destination square when the squares are numbered sequentially row
 *          by row.
 * https://oeis.org/A278828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini, full, easy
 * Formula:   a(n) = the possible differences Y – X for a king move on an 8×8
 *            board: a(1) = –9, a(2) = –8, a(3) = –7, a(4) = –1, a(5) = 1,
 *            a(6) = 7, a(7) = 8, a(8) = 9. For n > 8 the value is defined to
 *            be 0 (no further terms)
 * Range:     i8 n <= 8, i16 n <= 8, i32 n <= 8, i64 n <= 8, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t vals[8] = { -9, -8, -7, -1, 1, 7, 8, 9 };
    if (n >= 1 && n <= 8)
        return vals[n - 1];
    /* Undefined for n > 8; return 0 to satisfy any future calls. */
    return 0;
}
