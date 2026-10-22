/*
 * A360970  Multiplicative with a(p^e) = e^3, p prime and e > 0.
 * https://oeis.org/A360970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = (∏_{p^e||n} e)^3, multiplicative with a(p^e) = e^3
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1; a(1) = 1 by convention (empty product) */
    idx_t temp = n;
    seq_t prod = 1;   /* product of prime‑factor exponents */

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            unsigned e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            prod *= e;        /* multiply by current exponent */
        }
    }

    /* If a prime factor greater than sqrt(n) remains, its exponent is 1;
       multiplying by 1 has no effect on prod. */

    seq_t cube = prod;
    cube *= prod;          /* prod^2 */
    cube *= prod;          /* prod^3 */
    return cube;
}
