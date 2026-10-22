/*
 * A400397  a(n) = Sum_{d_1|n, d_2|n} gcud(d_1, d_2), where gcud is the
 *          greatest common unitary divisor.
 * https://oeis.org/A400397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d1|n, d2|n} gcud(d1,d2); Multiplicative with a(p^e)
 *            = (p^(e+1)-1)/(p-1) + e*(e+1)
 * Range:     u8 n <= 16, u16 n <= 2475, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t r = 1;
    while (exp > 0) {
        r *= base;
        exp--;
    }
    return r;
}

/* Compute the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) {            /* offset is 1, but safeguard for n=0 */
        return 0;
    }
    if (n == 1) {
        return (seq_t)1;
    }

    seq_t result = 1;
    idx_t temp = n;

    /* Trial division to factor n.  n is small (≤ 10⁴ in tests), so this is fast. */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p != 0) {
            continue;
        }
        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            e++;
        }
        /* Compute term for p^e. */
        seq_t p_pow = pow_seq((seq_t)p, e + 1);      /* p^(e+1) */
        seq_t geom = (p_pow - 1) / ((seq_t)p - 1);    /* (p^(e+1)-1)/(p-1) */
        seq_t term = geom + (seq_t)e * (e + 1);        /* a(p^e) */
        result *= term;
    }

    /* Remaining prime factor (if any). */
    if (temp > 1) {
        idx_t e = 1;
        seq_t p_pow = pow_seq((seq_t)temp, e + 1);    /* temp^(1+1) */
        seq_t geom = (p_pow - 1) / ((seq_t)temp - 1);
        seq_t term = geom + (seq_t)e * (e + 1);
        result *= term;
    }

    return result;
}
