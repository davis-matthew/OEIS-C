/*
 * A385560  Period of {binomial(N,9) mod n: N in Z}. Also, period of
 *          {binomial(N,10) mod n: N in Z}.
 * https://oeis.org/A385560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n * gcd(6, n) * gcd(210, n) * (2 - (n mod 2))
 * Range:     u8 n <= 5, u16 n <= 167, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Local GCD for seq_t (works for arbitrary‑precision as well). */
static inline seq_t local_gcd(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* OEIS A385560 – period of binomial(N,9) mod n (and binomial(N,10)). */
seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t g6   = local_gcd(sn, (seq_t)6);
    seq_t g210 = local_gcd(sn, (seq_t)210);
    seq_t parity = (seq_t)(2 - (n & 1));   /* 2 if n even, 1 if n odd */

    seq_t res = sn * g6;
    res *= g210;
    res *= parity;
    return res;
}
