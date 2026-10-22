/*
 * A324028  One of the two successive approximations up to 5^n for the 5-adic
 *          integer sqrt(-6). This is the 3 (mod 5) case (except for n = 0).
 * https://oeis.org/A324028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; for n>0, a(n) is the unique integer in [0,5^n‑1] such
 *            that a(n)^2 ≡ –6 (mod 5^n) and a(n) ≡ 3 (mod 5). The code
 *            implements Hensel lifting: Starting from a(1)=3, lift a solution
 *            modulo 5^k to modulo 5^(k+1). The recurrence is a_{k+1} = a_k +
 *            t·5^k where t = (5 – (((a_k^2+6)/5^k) mod 5))...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool if needed */

/* multiply a * b modulo mod without overflow (works for both
   native integer types and arbitrary‑precision integers).  */
static seq_t mul_mod(seq_t a, seq_t b, seq_t mod) {
    seq_t result = 0;
    a %= mod;
    while (b > (seq_t)0) {
        if (b & (seq_t)1) {
            result = (result + a) % mod;
        }
        a = (a << 1) % mod;
        b >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* a(0) is a special case */
    if (n == 0) {
        return (seq_t)0;
    }

    seq_t x   = (seq_t)3;   /* a(1) */
    seq_t pow5 = (seq_t)5;  /* current modulus 5^k (k starts at 1) */

    for (idx_t k = 1; k < n; ++k) {
        /* modulus for the next step: 5^(k+1) = pow5 * 5 */
        seq_t mod = pow5 * (seq_t)5;

        /* compute (x^2 + 6) modulo mod safely */
        seq_t res = (mul_mod(x, x, mod) + (seq_t)6) % mod;

        /* a = (res / pow5) mod 5  (res < 5*pow5, so quotient <5) */
        seq_t a_term = (res / pow5) % (seq_t)5;

        /* t = -(a_term) mod 5 */
        seq_t t = ((seq_t)5 - a_term) % (seq_t)5;

        /* lift to next modulus */
        x = x + t * pow5;
        pow5 = mod;
    }

    return x;
}
