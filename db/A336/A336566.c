/*
 * A336566  a(n) = gcd(A336563(n), A336564(n)) = gcd(A057723(n)-n,
 *          n-A308135(n)).
 * https://oeis.org/A336566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A057723(n)-n, n-A308135(n)), with A057723(n) =
 *            prod_{p^e||n} (p+...+p^e), A308135(n) = sigma(n) - A057723(n);
 *            gcd taken of absolute values
 * Range:     u8 n <= 256, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 105 terms through n = 9710 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static seq_t A336566_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    uint64_t m = (uint64_t)n;
    seq_t sigma = (seq_t)1;
    seq_t core = (seq_t)1;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            seq_t sum_all = (seq_t)1;
            seq_t pw = (seq_t)1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                sum_all += pw;
            }
            sigma *= sum_all;
            core *= (sum_all - (seq_t)1);
        }
    }
    if (m > 1) {
        sigma *= ((seq_t)m + (seq_t)1);
        core *= (seq_t)m;
    }

    seq_t nn = (seq_t)n;
    seq_t d1 = core - nn;            /* core >= n always */
    seq_t S = sigma - core;          /* sum of non-coreful divisors */
    seq_t d2 = (nn >= S) ? (nn - S) : (S - nn);
    return A336566_gcd(d1, d2);
}
