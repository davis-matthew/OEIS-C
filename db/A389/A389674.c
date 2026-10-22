/*
 * A389674  If n is a power of 10, then leave n untouched, else rewrite n in
 *          the base given by the sum of its digits, and 'read' those new
 *          digits in base 10.
 * https://oeis.org/A389674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = if sum of decimal digits of n is 1 then n, else interpret
 *            the base-(sum) digits of n as a decimal number
 * Range:     u8 n <= 10, u16 n <= 100, u32 n <= 1099, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t nseq = (seq_t)n;
    int sum = 0;
    idx_t t = n;
    while (t > 0) {
        sum += t % 10;
        t /= 10;
    }
    if (sum == 1)
        return nseq;
    seq_t res = 0;
    seq_t pow10 = 1;
    seq_t tmp = nseq;
    seq_t base = (seq_t)sum;
    while (tmp > 0) {
        int digit = (int)(tmp % base);
        res += (seq_t)digit * pow10;
        pow10 *= (seq_t)10;
        tmp /= base;
    }
    return res;
}
