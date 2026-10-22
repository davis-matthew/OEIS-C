/*
 * A370667  Largest pandigital number whose n-th power contains each digit
 *          (0-9) exactly n times.
 * https://oeis.org/A370667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, fini, full, nonn
 * Formula:   a(n) is the largest 10‑digit pandigital number whose n‑th power
 *            contains each decimal digit 0‑9 exactly n times. For n > 6 no
 *            such number exists, so a(n) = 0
 * Range:     u8 none, u16 none, u32 none, u64 n <= 6, u128 n <= 6, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 6) {
        switch (n) {
            case 1: return (seq_t)9876543210ULL;
            case 2: return (seq_t)9876124053ULL;
            case 3: return (seq_t)9863527104ULL;
            case 4: return (seq_t)9846032571ULL;
            case 5: return (seq_t)9847103256ULL;
            case 6: return (seq_t)9247560381ULL;
        }
    }
    return (seq_t)0;
}
