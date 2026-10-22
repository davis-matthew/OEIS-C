/*
 * A344341  Gray-code Niven numbers: numbers divisible by the number of 1's in
 *          their binary reflected Gray code (A005811).
 * https://oeis.org/A344341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 112, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes n‑th Gray‑code Niven number:
 * a(n) is the n‑th smallest integer m such that m is divisible by
 * the number of 1's in the binary reflected Gray code of m,
 * i.e. popcount(m ^ (m >> 1)) divides m. */
seq_t a(idx_t n)
{
    seq_t m = 1;
    idx_t count = 0;
    while (1) {
        seq_t gray = m ^ (m >> 1);
        seq_t ones = 0;
        seq_t t = gray;
        while (t) {
            ones++;
            t = t & (t - 1);
        }
        if (ones != 0 && (m % ones) == 0) {
            count++;
            if (count == n) return m;
        }
        m++;
    }
}
