/*
 * A373438  Expansion of Sum_{k>=1} k * x^(3^(k-1)) / (1 - x^(3^(k-1))).
 * https://oeis.org/A373438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = (v_3(n)+1)*(v_3(n)+2)/2 where v_3(n) is the exponent of 3
 *            dividing n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9062 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t exp3(idx_t n)
{
    idx_t e = 0;
    while (n % 3 == 0) {
        ++e;
        n /= 3;
    }
    return e;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* values below offset are irrelevant */
    idx_t e = exp3(n);                         /* exponent of 3 in n */
    unsigned long long tmp = (unsigned long long)(e + 1) * (e + 2);
    return (seq_t)(tmp / 2);                   /* triangular number of (e+1) */
}
