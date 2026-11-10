/*
 * A325634  a(n) = A091255(n, sigma(n)).
 * https://oeis.org/A325634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A091255(n, sigma(n)) = gcd over GF(2)[x] of the
 *            polynomials whose coefficients are the binary digits of n and
 *            sigma(n)
 * Range:     u8 n <= 135, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9899 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A325634_deg(seq_t p)
{
    int d = -1;
    while (p != 0) {
        p >>= 1;
        d++;
    }
    return d;
}

static seq_t A325634_pmod(seq_t a, seq_t b)
{
    int db = A325634_deg(b);
    int da = A325634_deg(a);
    while (a != 0 && da >= db) {
        a ^= (b << (da - db));
        da = A325634_deg(a);
    }
    return a;
}

static seq_t A325634_pgcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = A325634_pmod(a, b);
        a = b;
        b = r;
    }
    return a;
}

static seq_t A325634_sigma(idx_t n)
{
    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p;
    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            seq_t term = 1;
            seq_t pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
            }
            result *= term;
        }
    }
    if (m > 1)
        result *= ((seq_t)m + 1);
    return result;
}

seq_t a(idx_t n)
{
    return A325634_pgcd((seq_t)n, A325634_sigma(n));
}
