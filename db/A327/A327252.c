/*
 * A327252  Balanced reversed ternary: Write n as ternary, reverse the order
 *          of the digits, then replace all 2's with (-1)'s.
 * https://oeis.org/A327252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look, base
 * Formula:   a(n) = Σ_{i=0}^{k-1} ((digit_{rev}(n)_i == 2) ? -1 :
 *            digit_{rev}(n)_i) * 3^i, where digit_{rev}(n) is the reversed
 *            base‑3 representation of n
 * Range:     i8 n <= 243, i16 n <= 9910, i32 n <= 9910, i64 n <= 9910,
 *            i128 n <= 9910, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n;
    unsigned char digits[64];
    unsigned long long len = 0;
    /* Collect base‑3 digits of n (least significant first). */
    while (m > 0) {
        digits[len++] = m % 3;
        m /= 3;
    }
    seq_t result = 0;
    seq_t power   = 1;
    /* Process digits from most significant to least — this is the
       reversed order required by the definition. */
    for (unsigned long long i = len; i > 0; --i) {
        int d   = digits[i-1];
        seq_t f = (d == 2) ? -1 : d;
        result += f * power;
        power   *= 3;
    }
    return result;
}
