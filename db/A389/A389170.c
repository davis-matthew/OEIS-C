/*
 * A389170  a(n) = A057723(n) - A048146(n), where A048146 is sum of
 *          non-unitary divisors and A057723 is sum of positive divisors of n
 *          that are divisible by every prime that divides n.
 * https://oeis.org/A389170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A057723(n) - A048146(n) A057723(n) = product over primes
 *            p^e || n of p*(p^e-1)/(p-1) A048146(n) = sum of all non‑unitary
 *            divisors of n
 * Range:     u8 n <= 22, u16 n <= 1123, u32 n <= 9762, u64 n <= 9762,
 *            u128 n <= 9762, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* small helper that returns p^e (e <= 64) as seq_t */
static seq_t pow_seq(seq_t base, int exp)
{
    seq_t res = 1;
    while (exp--)
        res *= base;
    return res;
}

/* computes a(n) for n >= 1, with external linkage */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)n;           /* a(1)=1, a(0)=0 for completeness */

    uint64_t N   = (uint64_t)n;      /* n fits in 64 bits for factorisation */
    uint64_t primes[64];
    int       exps[64];
    int       fcnt = 0;

    /* trial division factorisation */
    uint64_t temp = N;

    if ((temp & 1) == 0) {
        int cnt = 0;
        while ((temp & 1) == 0) { temp >>= 1; cnt++; }
        primes[fcnt] = 2;
        exps[fcnt]   = cnt;
        fcnt++;
    }

    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int cnt = 0;
            while (temp % p == 0) { temp /= p; cnt++; }
            primes[fcnt] = p;
            exps[fcnt]   = cnt;
            fcnt++;
        }
    }
    if (temp > 1) {
        primes[fcnt] = temp;
        exps[fcnt]   = 1;
        fcnt++;
    }

    /* compute sigma(n), sigma*(n), and A057723 */
    seq_t sigma      = 1;   /* ordinary divisor sum */
    seq_t sigma_star = 1;   /* unitary divisor sum */
    seq_t divSum     = 1;   /* sum of divisors divisible by every prime of n */

    for (int i = 0; i < fcnt; i++) {
        seq_t p  = (seq_t)primes[i];
        int   e  = exps[i];

        seq_t pow_pe  = pow_seq(p, e);      /* p^e   */
        seq_t pow_pe1 = pow_seq(p, e + 1);  /* p^(e+1) */

        /* ordinary sigma part: (p^(e+1)-1)/(p-1) */
        seq_t sigma_part = (pow_pe1 - 1) / (p - 1);
        sigma *= sigma_part;

        /* unitary sigma part: 1 + p^e */
        sigma_star *= (1 + pow_pe);

        /* A057723 part: (p^(e+1)-p)/(p-1) = p*(p^e-1)/(p-1) */
        seq_t div_part = (pow_pe1 - p) / (p - 1);
        divSum *= div_part;
    }

    seq_t a_val = divSum - (sigma - sigma_star);
    return a_val;
}
