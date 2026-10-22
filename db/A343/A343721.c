/*
 * A343721  a(n) is the number of starting residues r modulo n from which
 *          repeated iterations of the mapping r -> r^2 mod n reach a fixed
 *          point.
 * https://oeis.org/A343721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of residues r modulo n that after repeated
 *            squaring r → r² (mod n) reach a fixed point (i.e., 0 or 1 modulo
 *            n). For n ≥ 1, a(n) = ∏_{p^e || n} (p^e − φ(p^e) +
 *            2^{v₂(φ(p^e))}), where φ is Euler's totient and v₂(x) is the
 *            exponent of 2 in x
 * Range:     u8 n <= 255, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but safe */
    seq_t result = 1;
    idx_t temp = n;

    /* factor n by trial division; n <= 10 000 in typical use cases */
    for (idx_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p != 0) continue;

        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }

        /* compute p^e, φ(p^e) and the contribution */
        seq_t power = 1;
        for (idx_t i = 0; i < e; ++i)
            power *= (seq_t)p;

        seq_t phi = power - power / (seq_t)p;        /* φ(p^e) = p^e - p^{e-1} */
        seq_t non_units = power - phi;              /* residues not units */

        /* find v₂(φ) */
        seq_t tmp_phi = phi;
        unsigned int t = 0;
        while ((tmp_phi & (seq_t)1) == (seq_t)0) {
            tmp_phi >>= 1;
            ++t;
        }
        seq_t two_power = ((seq_t)1) << t;          /* 2^{v₂(φ)} */

        seq_t a_p_e = non_units + two_power;        /* contribution of p^e */
        result *= a_p_e;
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        seq_t p = temp;
        seq_t power = p;                          /* p¹ */
        seq_t phi = power - (power / p);          /* = p-1 */
        seq_t non_units = power - phi;            /* = 1 */

        seq_t tmp_phi = phi;
        unsigned int t = 0;
        while ((tmp_phi & (seq_t)1) == (seq_t)0) {
            tmp_phi >>= 1;
            ++t;
        }
        seq_t two_power = ((seq_t)1) << t;
        seq_t a_p_e = non_units + two_power;
        result *= a_p_e;
    }

    return result;
}
