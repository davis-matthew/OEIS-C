/*
 * A389184  Number of mutual-visibility sets in the n-prism graph.
 * https://oeis.org/A389184
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = For odd n >= 9: (7*n^6 + 135*n^5 + 730*n^4 + 11370*n^3 -
 *            43937*n^2 + 83535*n + 5760)/5760 For even n >= 10: (7*n^6 +
 *            210*n^5 + 1660*n^4 + 14040*n^3 - 117632*n^2 + 384000*n +
 *            5760)/5760 (Special values are supplied for 3 ≤ n ≤ 10.)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 25, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 54 terms through n = 996 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t even_polynomial(seq_t sn)
{
    const seq_t c7   = 7,      c210  = 210,     c1660  = 1660;
    const seq_t c14040 = 14040, c117632 = 117632,   c384000 = 384000;
    const seq_t c5760 = 5760;

    seq_t p2 = sn * sn;            /* n^2 */
    seq_t p3 = p2 * sn;            /* n^3 */
    seq_t p4 = p3 * sn;            /* n^4 */
    seq_t p5 = p4 * sn;            /* n^5 */
    seq_t p6 = p5 * sn;            /* n^6 */

    seq_t num = c7 * p6
              + c210 * p5
              + c1660 * p4
              + c14040 * p3
              - c117632 * p2
              + c384000 * sn
              + c5760;

    return num / 5760;
}

static seq_t odd_polynomial(seq_t sn)
{
    const seq_t c7   = 7,      c135  = 135,     c730  = 730;
    const seq_t c11370 = 11370, c43937 = 43937,   c83535 = 83535;
    const seq_t c5760 = 5760;

    seq_t p2 = sn * sn;            /* n^2 */
    seq_t p3 = p2 * sn;            /* n^3 */
    seq_t p4 = p3 * sn;            /* n^4 */
    seq_t p5 = p4 * sn;            /* n^5 */
    seq_t p6 = p5 * sn;            /* n^6 */

    seq_t num = c7 * p6
              + c135 * p5
              + c730 * p4
              + c11370 * p3
              - c43937 * p2
              + c83535 * sn
              + c5760;

    return num / 5760;
}

seq_t a(idx_t n)
{
    /* Handle the first few values explicitly (offset = 3) */
    if (n == 3) return 54;
    if (n == 4) return 181;
    if (n == 5) return 311;
    if (n == 6) return 892;
    if (n == 7) return 1247;
    if (n == 8) return 3153;
    if (n == 9) return 3814;   /* odd formula works for 9 */
    if (n == 10) return 8806;

    /* For n >= 11 use the appropriate polynomial */
    if ((n & 1) == 1) { /* odd */
        return odd_polynomial((seq_t)n);
    } else {            /* even */
        return even_polynomial((seq_t)n);
    }
}
