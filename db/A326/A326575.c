/*
 * A326575  Expansion of Sum_{k>=1} k * x^k * (1 + x^(2*k)) / (1 + x^(2*k) +
 *          x^(4*k)).
 * https://oeis.org/A326575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Σ_{d|n, n/d≡1 (mod 6)} d – Σ_{d|n, n/d≡5 (mod 6)} d *
 *            Computed multiplicatively using * a(p^e) = p^e if p < 5 * a(p^e)
 *            = (p^(e+1) – (−1)^(e+1))/(p+1) if p ≡ 5 (mod 6) * a(p^e) =
 *            (p^(e+1) – 1)/(p–1) if p ≡ 1 (mod 6)
 * Range:     u8 n <= 16, u16 n <= 640, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Small power routine for seq_t.  Suitable for both C and C++. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1)
            res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* safety, though n>=1 by specification */

    idx_t temp = n;
    seq_t prod = 1;

    /* trial division up to sqrt(n) */
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p != 0)
            continue;
        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }

        seq_t contrib;
        if (p < 5) {                       /* p = 2 or 3 */
            contrib = pow_seq((seq_t)p, (idx_t)e);
        } else if (p % 6 == 5) {            /* p ≡ 5 (mod 6) */
            seq_t pow_val = pow_seq((seq_t)p, (idx_t)(e + 1));
            seq_t num;
            if ((e + 1) % 2 == 0)           /* (−1)^(e+1) = 1 */
                num = pow_val - 1;
            else                            /* (−1)^(e+1) = –1 */
                num = pow_val + 1;
            contrib = num / ((seq_t)(p + 1));
        } else if (p % 6 == 1) {            /* p ≡ 1 (mod 6) */
            seq_t pow_val = pow_seq((seq_t)p, (idx_t)(e + 1));
            seq_t num = pow_val - 1;
            contrib = num / ((seq_t)(p - 1));
        } else {                            /* should never occur */
            contrib = 1;
        }
        prod *= contrib;
    }

    /* remaining prime factor > sqrt(n) */
    if (temp > 1) {
        idx_t p = temp;
        idx_t e = 1;
        seq_t contrib;
        if (p < 5) {
            contrib = pow_seq((seq_t)p, (idx_t)e);
        } else if (p % 6 == 5) {
            seq_t pow_val = pow_seq((seq_t)p, (idx_t)(e + 1));
            seq_t num;
            if ((e + 1) % 2 == 0)
                num = pow_val - 1;
            else
                num = pow_val + 1;
            contrib = num / ((seq_t)(p + 1));
        } else { /* p % 6 == 1 */
            seq_t pow_val = pow_seq((seq_t)p, (idx_t)(e + 1));
            seq_t num = pow_val - 1;
            contrib = num / ((seq_t)(p - 1));
        }
        prod *= contrib;
    }

    return prod;
}
