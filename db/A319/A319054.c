/*
 * A319054  Maximum product of an aperiodic integer partition of n.
 * https://oeis.org/A319054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for n=1,2,3 the values are 1,2,3. For n > 3: if n ≡ 1 (mod 3):
 *            a(n) = 4 * 3^((n-4)/3) if n ≡ 2 (mod 3): a(n) = 2 * 3^((n-2)/3)
 *            if n ≡ 0 (mod 3): a(n) = 8 * 3^((n-6)/3) (n>3)
 * Range:     u8 n <= 15, u16 n <= 30, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)2;
    if (n == (idx_t)3) return (seq_t)3;

    seq_t result = (seq_t)1;
    idx_t exp;
    if ((n % (idx_t)3) == (idx_t)1) {
        exp = (n - (idx_t)4) / (idx_t)3;
        for (idx_t i = (idx_t)0; i < exp; ++i) result *= (seq_t)3;
        result *= (seq_t)4;
    } else if ((n % (idx_t)3) == (idx_t)2) {
        exp = (n - (idx_t)2) / (idx_t)3;
        for (idx_t i = (idx_t)0; i < exp; ++i) result *= (seq_t)3;
        result *= (seq_t)2;
    } else { /* n % 3 == 0 and n > 3 */
        exp = (n - (idx_t)6) / (idx_t)3;
        for (idx_t i = (idx_t)0; i < exp; ++i) result *= (seq_t)3;
        result *= (seq_t)8;
    }
    return result;
}
