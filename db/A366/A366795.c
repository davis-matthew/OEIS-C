/*
 * A366795  a(n) = A344695(A005940(1+n)), where A344695(n) = gcd(psi(n),
 *          sigma(n)), and A005940 is the Doudna sequence.
 * https://oeis.org/A366795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(psi(m), sigma(m)) with m = A005940(n+1), the Doudna
 *            number; sigma and psi are computed multiplicatively from the
 *            exponents read off the binary digits of n
 * Range:     u8 n <= 51, u16 n <= 1353, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 102 terms through n = 9356 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    uint64_t primes[70];
    int np = 0;
    for (uint64_t c = 2; np < 70; ++c) {
        bool ok = true;
        for (int j = 0; j < np && primes[j] * primes[j] <= c; ++j)
            if (c % primes[j] == 0) { ok = false; break; }
        if (ok) primes[np++] = c;
    }

    unsigned ex[70];
    for (int j = 0; j < 70; ++j) ex[j] = 0;

    idx_t m = n;
    int i = 1;
    while (m > 0) {
        if (m & 1) {
            ex[i - 1]++;
            m = (m - 1) / 2;
        } else {
            m /= 2;
            i++;
        }
    }

    seq_t sg = 1, ps = 1;
    for (int j = 0; j < 70; ++j) {
        if (ex[j] == 0) continue;
        seq_t p = (seq_t)primes[j];
        seq_t term = 1, pw = 1;
        for (unsigned k = 0; k < ex[j]; ++k) {
            pw *= p;
            term += pw;
        }
        sg *= term;
        seq_t pe = 1;
        for (unsigned k = 1; k < ex[j]; ++k) pe *= p;
        ps *= pe * (p + 1);
    }
    return gcd(sg, ps);
}
