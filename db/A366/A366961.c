/*
 * A366961  Numbers whose difference between the largest and smallest digits
 *          is equal to 4.
 * https://oeis.org/A366961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force scan: test each integer for difference between largest
 * and smallest digits equal to 4. */

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t x = 0; ; ++x) {
        seq_t num = x;
        int min_d = 9, max_d = 0;
        while (num > 0) {
            int d = (int)(num % 10);
            if (d < min_d) min_d = d;
            if (d > max_d) max_d = d;
            num /= 10;
        }
        if (max_d - min_d == 4) {
            ++count;
            if (count == n) {
                return (seq_t)x;
            }
        }
    }
}
