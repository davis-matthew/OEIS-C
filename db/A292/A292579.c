/*
 * A292579  Inverse to A055643: Consider n as a number written in base 60,
 *          using 2 decimal digits for each base-60 digit (which we allow to
 *          be in the range 0 .. 99), and write n in base 10.
 * https://oeis.org/A292579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{k≥0} (n / 10^{2k} % 100) * 60^k (i.e. interpret n as a
 *            base‑60 number where each two decimal digits form a digit)
 * Range:     u8 n <= 109, u16 n <= 109, u32 n <= 109, u64 n <= 109,
 *            u128 n <= 109, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t pow60  = 1;
    idx_t t = n;
    while (t > 0) {
        seq_t group = (seq_t)(t % 100);
        result += group * pow60;
        pow60 *= 60;
        t /= 100;
    }
    return result;
}
