/*
 * A341005  Numbers whose sum of even digits and sum of odd digits differ by
 *          4.
 * https://oeis.org/A341005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 4527, u32 n <= 5001, u64 n <= 5001,
 *            u128 n <= 5001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose sum of even digits and sum of odd digits differ by 4.
   The function scans integers from 1 upwards, keeps a running count
   of numbers satisfying |sum_even - sum_odd| == 4, and returns the
   n-th such number. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                   /* undefined, but safe */
    idx_t count = 0;
    seq_t num = 1;                           /* start from 1 */
    for (;;) {
        seq_t temp = num;
        int diff = 0;                       /* difference even - odd */
        while (temp > 0) {
            unsigned digit = (unsigned)(temp % 10);
            if (digit & 1)  diff -= digit; else diff += digit;
            temp /= 10;
        }
        if (diff == 4 || diff == -4) {
            count++;
            if (count == n) return num;
        }
        ++num;
    }
}
