/*
 * A271350  a(n) = 3^n mod 83.
 * https://oeis.org/A271350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 83
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = 3;
    const seq_t mod = 83;
    idx_t e = n;

    while (e > 0) {
        if (e & 1) {
            unsigned long long temp = (unsigned long long)result * (unsigned long long)base;
            result = (seq_t)(temp % (unsigned long long)mod);
        }
        unsigned long long temp2 = (unsigned long long)base * (unsigned long long)base;
        base = (seq_t)(temp2 % (unsigned long long)mod);
        e >>= 1;
    }

    return result;
}
