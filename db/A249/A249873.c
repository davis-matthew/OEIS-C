/*
 * A249873  In decimal representation: replace each even positioned digit with
 *          the digit sum of its double.
 * https://oeis.org/A249873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   replace each even‑positioned decimal digit (positions counted
 *            from the right, starting at 1) by the digit sum of its double
 * Range:     u8 n <= 229, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    seq_t pow = 1;
    idx_t pos = 1;
    while (n > 0) {
        seq_t d = n % 10;          /* least‑significant decimal digit */
        seq_t nd = d;
        if (pos % 2 == 0) {        /* even position – transform */
            seq_t tmp = nd * 2;
            nd = (tmp < 10) ? tmp : (tmp - 9); /* digit sum of 2*d */
        }
        res += nd * pow;
        pow *= 10;
        n /= 10;
        pos++;
    }
    return res;
}
