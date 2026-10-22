/*
 * A350396  Numbers m such that there are two or more terms with the largest
 *          coefficient in Product_{k=1..m} (1 + x^k).
 * https://oeis.org/A350396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(3)=4, a(4)=5, a(5)=6, a(6)=7; for n≥7 let r =
 *            n-7 and k = 2 + ⌊r/2⌋. Then a(n) = 4*k + 1 if r is even, else
 *            a(n) = 4*k + 2
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t small[6] = {1, 2, 4, 5, 6, 7};

    if (n <= 6) {
        return small[n - 1];
    }

    /* r = n - 7 (as idx_t) */
    seq_t r = (seq_t)(n - 7);

    /* k = 2 + floor(r / 2) */
    seq_t k = 2 + r / 2;

    if ((r & 1) == 0) {
        /* r even */
        return 4 * k + 1;
    } else {
        /* r odd */
        return 4 * k + 2;
    }
}
