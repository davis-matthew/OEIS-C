/*
 * A271379  a(n) = 5^n mod 101.
 * https://oeis.org/A271379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n mod 101
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t MOD  = 101;
    static const seq_t BASE = 5;
    seq_t result = 1;
    seq_t power  = BASE;
    idx_t exp    = n;

    while (exp > 0) {
        if (exp & 1) {
            result = (result * power) % MOD;
        }
        power = (power * power) % MOD;
        exp >>= 1;
    }
    return result;
}
