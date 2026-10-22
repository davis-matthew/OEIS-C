/*
 * A372961  a(n) = Sum_{1 <= x_1, x_2, x_3, x_4 <= n} n/gcd(x_1, x_2, x_3,
 *          x_4, n).
 * https://oeis.org/A372961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} d * J_4(d) Multiplicative formula: a(p^e) =
 *            (p^(5e+5) - p^(5e+1) + p - 1) / (p^5-1) = (p^5)^e + (1-p) *
 *            sum_{k=0}^{e-1} (p^5)^k This implementation uses 128‑bit integer
 *            arithmetic to avoid overflow for the intermediate values when
 *            seq_t is a 64‑bit unsigned integer
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 7132,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t prime_power_term(idx_t p, idx_t e)
{
    /* compute p^5 in 128 bits */
    unsigned __int128 p5 = 1;
    for (idx_t i = 0; i < 5; ++i) p5 *= (unsigned __int128)p;

    /* sum_{k=0}^{e-1} (p^5)^k */
    unsigned __int128 power = 1;   /* (p^5)^k */
    __int128 sum_lt = 0;          /* signed to allow negative product */

    for (idx_t k = 0; k < e; ++k) {
        sum_lt += (__int128)power;   /* accumulate k-th power */
        power *= p5;                  /* prepare next power (p^5)^(k+1) */
    }

    /* power now equals (p^5)^e */
    __int128 val = (__int128)power;
    __int128 coeff = (__int128)1 - (__int128)p;
    val += coeff * sum_lt;          /* a(p^e) value (exact integer) */

    return (seq_t)val;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;          /* a(1) = 1 */

    idx_t temp = n;
    __int128 acc = 1;              /* use 128‑bit accumulator */

    /* factor 2 */
    if (temp % 2 == 0) {
        idx_t e = 0;
        while (temp % 2 == 0) { temp /= 2; ++e; }
        acc *= (__int128)prime_power_term(2, e);
    }

    /* odd factors */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) { temp /= p; ++e; }
            acc *= (__int128)prime_power_term(p, e);
        }
    }

    /* remaining prime factor > sqrt(n) */
    if (temp > 1) {
        acc *= (__int128)prime_power_term(temp, 1);
    }

    return (seq_t)acc;
}
