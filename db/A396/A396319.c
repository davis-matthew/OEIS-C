/*
 * A396319  Denominator of the largest abundancy of any proper divisor of n as
 *          a fraction of the abundancy of n.
 * https://oeis.org/A396319
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of max_{p^e‖n} (p^{e+1}-p)/(p^{e+1}-1)
 * Range:     u8 n <= 16, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*             which equals (p^{e+1}-1)/(p-1) for the prime power achieving the maximum */

#include <stdint.h>
#include <stdbool.h>

/* integer power: base^exp for small exp (≤ 64) */
static inline seq_t pow_seq_uint(uint64_t base, unsigned exp)
{
    seq_t result = 1;
    for (unsigned i = 0; i < exp; ++i)
        result *= (seq_t)base;
    return result;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                      /* undefined below offset */

    uint64_t m       = (uint64_t)n;           /* original number for reference */
    uint64_t temp    = m;                     /* variable for factorisation */

    seq_t best_num   = 0;                     /* numerator of best ratio */
    seq_t best_den   = 1;                     /* denominator of best ratio */
    uint64_t best_p  = 0;                     /* prime giving best ratio */
    unsigned best_e  = 0;                     /* exponent of best prime */

    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            unsigned e = 0;
            while (temp % p == 0)
            {
                ++e;
                temp /= p;
            }

            seq_t pow  = pow_seq_uint(p, e + 1);     /* p^{e+1}   */
            seq_t numer = pow - p;                  /* p^{e+1} - p   */
            seq_t denom = pow - 1;                  /* p^{e+1} - 1   */

            /* compare numer/denom with current best ratio */
            if (best_num * denom < numer * best_den)
            {
                best_num = numer;
                best_den = denom;
                best_p  = p;
                best_e  = e;
            }
        }
    }

    /* any remaining factor > sqrt may be prime */
    if (temp > 1)
    {
        uint64_t p = temp;          /* remaining prime factor */
        unsigned e = 1;             /* exponent is 1 */

        seq_t pow  = pow_seq_uint(p, e + 1);
        seq_t numer = pow - p;
        seq_t denom = pow - 1;

        if (best_num * denom < numer * best_den)
        {
            best_num = numer;
            best_den = denom;
            best_p   = p;
            best_e   = e;
        }
    }

    /* denominator of reduced fraction is (p^{e+1}-1)/(p-1) for the chosen prime power */
    seq_t pow_best = pow_seq_uint(best_p, best_e + 1);
    seq_t result   = (pow_best - 1) / (best_p - 1);

    return result;
}
