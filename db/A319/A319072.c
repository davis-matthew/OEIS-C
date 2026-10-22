/*
 * A319072  a(n) is the sum of the non-bi-unitary divisors of n.
 * https://oeis.org/A319072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of non‑bi‑unitary divisors of n = sigma(n) –
 *            sigma_bi(n) where sigma(n) is the sum of all divisors,
 *            sigma_bi(n) is the sum of bi‑unitary divisors. For a prime power
 *            p^e: e odd -> sigma_bi(p^e) = sigma(p^e) e even -> sigma_bi(p^e)
 *            = sigma(p^e) – p^(e/2)
 * Range:     u8 n <= 67, u16 n <= 6353, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* helper: power of an unsigned 64‑bit integer to an integer exponent
   (exponent is small, e ≤ log₂n).  Result stored in seq_t. */
static seq_t pow_uint(uint64_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= (seq_t)base;
    return result;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* a(0) and a(1) are 0 */
    seq_t sigma     = 1;          /* product of local sigma(p^e) */
    seq_t bi_sigma  = 1;          /* product of local sigma_bi(p^e) */
    idx_t temp = n;

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            seq_t power_e1 = pow_uint(p, e + 1);                 /* p^(e+1) */
            seq_t sigma_pe = (power_e1 - 1) / ((seq_t)p - 1);     /* sigma(p^e) */

            sigma    *= sigma_pe;

            seq_t local;
            if (e & 1)                 /* e odd */
                local = sigma_pe;
            else {                      /* e even */
                local = sigma_pe - pow_uint(p, e / 2);
            }
            bi_sigma *= local;
        }
    }

    /* Remaining prime factor > sqrt(n) (exponent 1) */
    if (temp > 1) {
        idx_t p = temp;
        /* e = 1 is odd */
        seq_t power_e1 = pow_uint(p, 2);                     /* p^(1+1) = p^2 */
        seq_t sigma_pe = (power_e1 - 1) / ((seq_t)p - 1);     /* sigma(p) = 1+p */

        sigma    *= sigma_pe;
        bi_sigma *= sigma_pe;  /* local = sigma_pe for odd exponent */
    }

    return sigma - bi_sigma;
}
