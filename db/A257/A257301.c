/*
 * A257301  Number of cubic nonresidues modulo n.
 * https://oeis.org/A257301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 300, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _pow_seq_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n - (# of distinct cubic residues modulo n)
 * (Number of cubic nonresidues modulo n)
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_pow_seq_uint.h"

/* Small integer gcd for seq_t values */
static seq_t _seq_gcd(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Count distinct cubic residues modulo p^e (p is prime, e>=1) */
static seq_t _cubic_residues_prime_power(uint64_t p, idx_t e)
{
    /* 0 is always a residue */
    seq_t total = 1;

    for (idx_t s = 0; 3 * s < e; ++s) {
        idx_t exp = e - 3 * s;          /* modulus exponent for the unit part */
        /* phi(p^exp) = p^(exp-1) * (p-1) */
        seq_t phi = (exp == 0) ? 1 : _pow_seq_uint(p, exp - 1) * (seq_t)(p - 1);
        seq_t g = _seq_gcd(phi, (seq_t)3);
        seq_t add = phi / g;            /* number of distinct residues of valuation 3*s */
        total += add;
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;   /* a(1)=0 */

    uint64_t m = n;
    seq_t residues = 1;        /* product of residue counts for each prime power */

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p != 0) continue;
        idx_t exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        residues *= _cubic_residues_prime_power(p, exp);
    }
    if (m > 1) {
        residues *= _cubic_residues_prime_power(m, 1);
    }

    return (seq_t)n - residues;
}
