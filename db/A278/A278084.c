/*
 * A278084  a(n) is 1/24 of the number of primitive integral quadruples with
 *          sum = 2*m and sum of squares = 6*m^2, where m = 2*n-1.
 * https://oeis.org/A278084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product over prime factors p^e of (p^(e-1) * b(p)), *
 *            where b(p) depends on p mod 20: * if p == 5 -> b = p * if p mod
 *            20 ∈ {11,13,17,19} -> b = p + 1 * if p mod 20 ∈ {1,3,7,9} -> b =
 *            p - 1 * This implements the conjectured multiplicativity of
 *            A278084
 * Range:     u8 n <= 123, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to m = 2*n - 1 (always odd) */
    idx_t m = 2 * n - 1;
    if (m <= 1)            /* a(1) = 1 */
        return 1;

    seq_t result = 1;
    idx_t temp   = m;

    /* Trial division by odd primes up to sqrt(temp) */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* Compute p^(e-1) as seq_t */
            seq_t pow_pe_minus1 = 1;
            for (idx_t i = 1; i < e; ++i)
                pow_pe_minus1 *= (seq_t)p;

            /* Determine b(p) */
            seq_t bp;
            if (p == 5)
                bp = 5;
            else {
                idx_t r = p % 20;
                if (r == 11 || r == 13 || r == 17 || r == 19)
                    bp = (seq_t)p + 1;
                else   /* r is 1,3,7,9 */
                    bp = (seq_t)p - 1;
            }

            result *= pow_pe_minus1 * bp;
        }
    }

    /* If any prime factor remains > 1, it occurs with exponent 1 */
    if (temp > 1) {
        idx_t p = temp;  /* remaining prime factor */
        seq_t bp;
        if (p == 5)
            bp = 5;
        else {
            idx_t r = p % 20;
            if (r == 11 || r == 13 || r == 17 || r == 19)
                bp = (seq_t)p + 1;
            else
                bp = (seq_t)p - 1;
        }
        result *= bp;
    }

    return result;
}
