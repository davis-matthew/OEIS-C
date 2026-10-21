/*
 * A246004  The duodecimal period of 1/n, or 0 if 1/n terminates.
 * https://oeis.org/A246004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 0 if n contains no prime factors other than 2 and 3.
 *            Otherwise a(n) is the multiplicative order of 12 modulo n',
 *            where n' = n divided by all factors 2 and 3
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*    For n>0 this computes the length of the repeating block of 1/n in base 12. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, but safe guard */

    /* remove all factors of 2 and 3 */
    idx_t m = n;
    while ((m % 2) == 0) m /= 2;
    while ((m % 3) == 0) m /= 3;

    /* if m == 1 then 1/n terminates in base 12 */
    if (m == 1) return 0;

    /* now m is coprime to 12; compute smallest k > 0 with 12^k ≡ 1 (mod m) */
    seq_t cur = 12 % (seq_t)m;   /* current power of 12 modulo m */
    seq_t period = 1;            /* at least 1 since m > 1 */

    while (cur != 1) {
        cur = (cur * 12) % (seq_t)m;
        ++period;
    }
    return period;
}
