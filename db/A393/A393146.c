/*
 * A393146  Halting time for Wolfram's 2-state 2-symbol Turing machine 3333
 *          when started with n on the tape.
 * https://oeis.org/A393146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 7; * a(n) = 3 if 4 | n * a(n) = a(n/2) if n % 4 == 2 *
 *            a(n) = a((n-1)/2) + 4 if n is odd and n > 1 * a(n) = 7 if n == 1
 *            * This recurrence reproduces the halting times for * Wolfram's
 *            2‑state 2‑symbol Turing machine 3333. The function * below
 *            implements the recurrence iteratively. The algor...
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;                     /* accumulated 4‑steps */

    while (1) {
        /* n is divisible by 4 → halting time 3 (base). */
        if ((n & 3) == 0) {
            return res + (seq_t)3;
        }

        /* n is even but not divisible by 4 → halving. */
        if ((n & 1) == 0) {
            n >>= 1;                    /* n = n / 2 */
            continue;
        }

        /* n is odd.  If n == 1 we have the base case 7. */
        if (n == 1) {
            return res + (seq_t)7;
        }

        /* For odd n > 1: add 4 and divide by 2 ((n-1)/2). */
        res += (seq_t)4;
        n >>= 1;                        /* (n-1)/2 == n >> 1 when n is odd */
    }
}
