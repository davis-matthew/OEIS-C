/*
 * A347128  a(n) = A018804(n) / A003557(n), where A018804 is Pillai's
 *          arithmetical function.
 * https://oeis.org/A347128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, look
 * Formula:   a(n) = A018804(n) / A003557(n) Multiplicative with a(p^e) =
 *            ((p-1)*e + p)
 * Range:     u8 n <= 65, u16 n <= 4289, u32 n <= 9960, u64 n <= 9960,
 *            u128 n <= 9960, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            result *= ((seq_t)(p - 1) * e + (seq_t)p);
        }
    }
    if (temp > 1) {  /* remaining prime factor */
        result *= ((seq_t)(temp - 1) + (seq_t)temp);
    }
    return result;
}
