/*
 * A328490  Dirichlet g.f.: zeta(s)^2 * zeta(s-2)^2.
 * https://oeis.org/A328490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} sigma_2(d) * sigma_2(n/d), * where sigma_2(m) =
 *            sum of squares of the divisors of m
 * Range:     u8 n <= 7, u16 n <= 59, u32 n <= 6719, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a for a single prime power p^e using the closed formula.
 * We perform all intermediate calculations in unsigned __int128
 * to avoid overflow for 64‑bit seq_t builds. */
static seq_t
a_prime_power(uint64_t p, idx_t e)
{
    unsigned __int128 p2   = (unsigned __int128)p * p;                 /* p^2          */
    unsigned __int128 term = (unsigned __int128)e * (p2 - 1) + p2 - 3; /* e*(p^2-1)+p^2-3 */
    /* Compute p^(2*e+4) using exponentiation by squaring. */
    unsigned __int128 powv = 1;
    unsigned __int128 base = p;
    idx_t exp = 2 * e + 4;
    while (exp) {
        if (exp & 1) powv *= base;
        base *= base;
        exp >>= 1;
    }
    unsigned __int128 num  = term * powv;                          /* part1 * p^(2e+4) */
    unsigned __int128 part2 = (unsigned __int128)e * (p2 - 1) + 3 * p2 - 1;
    num += part2;                                                  /* add part2    */
    unsigned __int128 denom = (p2 - 1);
    denom = denom * denom * denom;                                 /* (p^2-1)^3    */
    unsigned __int128 res128 = num / denom;
    return (seq_t)res128;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n=0, return 0 for safety */

    seq_t  res   = 1;
    idx_t  temp  = n;
    idx_t  e;

    /* Handle factor 2 separately for speed. */
    if ((temp & 1) == 0) {
        e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++e;
        }
        res *= a_prime_power(2, e);
    }

    /* Trial division by odd numbers up to sqrt(temp). */
    for (idx_t d = 3; d * d <= temp; d += 2) {
        if (temp % d == 0) {
            e = 0;
            while (temp % d == 0) {
                temp /= d;
                ++e;
            }
            res *= a_prime_power((uint64_t)d, e);
        }
    }

    /* If a prime factor remains > sqrt(n). */
    if (temp > 1) {
        res *= a_prime_power((uint64_t)temp, 1);
    }

    return res;
}
