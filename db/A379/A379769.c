/*
 * A379769  a(n) = (A006218(n) mod n).
 * https://oeis.org/A379769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (sum_{k=1}^n floor(n/k)) mod n
 * Range:     u8 n <= 59, u16 n <= 7428, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where the sum is the divisor summatory function A006218(n). */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* for safety; offset starts at 1 */
    seq_t sum = 0;
    idx_t i = 1;
    while (i <= n) {
        idx_t q = n / i;                 /* floor(n / i) */
        idx_t next = n / q;              /* last i with same quotient q */
        /* contribution of all i in [i, next]: q * (next - i + 1) */
        sum += (seq_t)q * (seq_t)(next - i + 1);
        i = next + 1;
    }
    return sum % (seq_t)n;
}
