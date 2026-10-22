/*
 * A303275  Decimal expansion of the value of 1 imperial (UK) gallon in
 *          liters.
 * https://oeis.org/A303275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   a(n) = digit at position n of 454609 (decimal digits of
 *            4.54609), all further digits are 0. For n = 1..6: a(n) = (454609
 *            / 10^(6-n)) % 10; for n>6: a(n) = 0
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t DIGITS = 454609;
    static const idx_t COUNT = 6;
    if (n == 0) return 0;
    if (n > COUNT) return 0;
    seq_t pow10 = 1;
    idx_t exp = COUNT - n;
    while (exp--) pow10 *= 10;
    return (DIGITS / pow10) % 10;
}
