/*
 * A298014  Coordination sequence of snub-632 tiling with respect to a
 *          trivalent node of type short-short-long.
 * https://oeis.org/A298014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=9, a(3)=15, a(4)=18; for n≥5, let
 *            k=floor(n/3): a(3k) = 20k – 3 a(3k+1) = 17k + 3 a(3k+2) = 17k +
 *            10
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
        case 0: return 1;
        case 1: return 3;
        case 2: return 9;
        case 3: return 15;
        case 4: return 18;
    }

    /* n ≥ 5 */
    idx_t k  = n / 3;        /* integer division */
    seq_t K  = (seq_t)k;     /* convert to seq_t for arithmetic */

    switch (n % 3) {
        case 0: return 20 * K - 3;
        case 1: return 17 * K + 3;
        case 2: return 17 * K + 10;
        default: return 0;   /* unreachable */
    }
}
