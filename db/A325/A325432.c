/*
 * A325432  Complement of A325431.
 * https://oeis.org/A325432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth positive integer k whose 3‑adic valuation and
 *            4‑adic valuation have opposite parity
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; undefined for n==0 */

    idx_t count = 0;
    idx_t k = 1;

    for (;;) {
        // exponent of 3 dividing k
        idx_t tmp = k;
        idx_t v3 = 0;
        while ((tmp % 3) == 0) {
            tmp /= 3;
            v3++;
        }

        // exponent of 2 dividing k
        tmp = k;
        idx_t v2 = 0;
        while ((tmp % 2) == 0) {
            tmp /= 2;
            v2++;
        }
        idx_t v4 = v2 / 2;          // exponent of 4 dividing k

        if ((v3 & 1) != (v4 & 1)) { // opposite parity
            count++;
            if (count == n) {
                return (seq_t)k;
            }
        }
        k++;
    }
}
