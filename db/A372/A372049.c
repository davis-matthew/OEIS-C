/*
 * A372049  a(n) is the index of the Lucas number that is the ratio of the sum
 *          of the first n Fibonacci numbers divided by the largest possible
 *          Fibonacci number.
 * https://oeis.org/A372049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + ((n-1)/4)*2 for n≡1 (mod 4), a(n) = 1 + ((n-2)/4)*2
 *            for n≡2 (mod 4), a(n) = 4 + ((n-4)/4)*2 for n≡0 (mod 4), a(3)=0,
 *            a(n) = 5 + ((n-7)/4)*2 for n≡3 (mod 4) and n≥7. This follows the
 *            recurrence a(n+4)=a(n)+2 for n>3, with special handling of a(3)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3) {
        return 0;
    }
    idx_t r = n % 4;
    if (r == 0) {
        r = 4;   /* treat remainder 0 as 4 */
    }
    switch (r) {
    case 1:
        return 1 + ((seq_t)((n - 1) / 4) * 2);
    case 2:
        return 1 + ((seq_t)((n - 2) / 4) * 2);
    case 4:
        return 4 + ((seq_t)((n - 4) / 4) * 2);
    case 3:
        /* n != 3 so n >= 7 */
        return 5 + ((seq_t)((n - 7) / 4) * 2);
    default:
        /* unreachable, but return 0 to satisfy compiler */
        return 0;
    }
}
