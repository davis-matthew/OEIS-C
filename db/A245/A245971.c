/*
 * A245971  Tower of 4s mod n.
 * https://oeis.org/A245971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A245971: a(n) = infinite tower of 4's modulo n
   Formula: a(n) = (4^(4^(4^( …  )))) mod n   (sufficiently many 4's) */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Auxiliary: modular exponentiation for arbitrary integral seq_t */
static seq_t mod_pow_seq(seq_t base, seq_t exp, seq_t mod)
{
    seq_t result = 1;
    seq_t b = base % mod;
    seq_t e = exp;
    while (e > 0) {
        if ((e & 1) != 0)
            result = (result * b) % mod;
        b = (b * b) % mod;
        e >>= 1;
    }
    return result;
}

/* Compute Euler's totient φ(n) as seq_t.  Uses trial division. */
static seq_t totient(idx_t n)
{
    seq_t res = (seq_t)n;
    idx_t temp = n;
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0)
                temp /= p;
            res -= res / (seq_t)p;
        }
    }
    if (temp > 1)
        res -= res / (seq_t)temp;
    return res;
}

/* Recursive evaluation of the infinite 4-tower modulo m. */
static seq_t tower_mod(idx_t m)
{
    if (m == 1)
        return 0;
    /* If m divides 4, the tower is 0 modulo m. */
    if ((seq_t)4 % (seq_t)m == 0)
        return 0;

    seq_t phi = totient(m);
    seq_t exp_mod = tower_mod((idx_t)phi);
    seq_t exponent;
    if ((seq_t)phi > 1)
        exponent = exp_mod + phi;  /* ensure cycle length for non‑coprime case */
    else
        exponent = exp_mod;

    return mod_pow_seq((seq_t)4, exponent, (seq_t)m);
}

/* Public entry point required by the build. */
seq_t a(idx_t n)
{
    return tower_mod(n);
}
