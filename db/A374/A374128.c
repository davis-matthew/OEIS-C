/*
 * A374128  a(n) = 1 if gcd(A075255(n), A374127(n)) = abs(A374127(n)),
 *          otherwise 0.
 * https://oeis.org/A374128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if gcd(A075255(n), A374127(n)) = |A374127(n)|, else 0;
 *            A075255(n) = n - sopfr(n), A374127(n) = sigma(n) - sopfr(n) - n
 * Range:     u8 n <= 1556, u16 n <= 100000, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 139 terms through n = 9871 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A374128_gcd(seq_t x, seq_t y)
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
    idx_t rem = n;
    seq_t sopfr = 0;
    seq_t sig = 1;
    for (idx_t p = 2; p <= rem / p; ++p) {
        if (rem % p == 0) {
            seq_t term = 1, pw = 1;
            while (rem % p == 0) {
                rem /= p;
                sopfr += (seq_t)p;
                pw *= (seq_t)p;
                term += pw;
            }
            sig *= term;
        }
    }
    if (rem > 1) {
        sopfr += (seq_t)rem;
        sig *= ((seq_t)rem + 1);
    }
    seq_t N = (seq_t)n;
    seq_t A = N - sopfr;
    seq_t s = sig - sopfr;
    seq_t B = (s >= N) ? (s - N) : (N - s);
    seq_t g = A374128_gcd(A, B);
    return (g == B) ? (seq_t)1 : (seq_t)0;
}
