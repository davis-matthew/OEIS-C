/*
 * A257629  Duplicate of A276729.
 * https://oeis.org/A257629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = A055642(n) - A193238(n) // number of nonprime decimal
 *            digits of n
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                 /* 0 has one nonprime digit */
    seq_t cnt = 0;
    idx_t x = n;
    while (x > 0)
    {
        unsigned d = x % 10;
        if (d != 2 && d != 3 && d != 5 && d != 7)
            cnt++;
        x /= 10;
    }
    return cnt;
}
