/*
 * A340365  a(n) = A005940(n) / gcd(A005940(n), A324106(n)), where A324106(n)
 *          is multiplicative with a(p^e) = A005940(p^e).
 * https://oeis.org/A340365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005940(n) / gcd(A005940(n), A324106(n)), where A324106
 *            is multiplicative with a(p^e) = A005940(p^e); computed via
 *            prime-exponent vectors
 * Range:     u8 n <= 92, u16 n <= 504, u32 n <= 16288, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 115 terms through n = 9165 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define A340365_MAXI 140

/* add exponent vector of A005940(m) into ex (indexed by prime index) */
static void A340365_add(uint64_t m1, int *ex)
{
    uint64_t m = m1 - 1;
    int i = 1;
    while (m > 0) {
        if (m & 1) {
            ex[i]++;
            m = (m - 1) / 2;
        } else {
            m /= 2;
            i++;
        }
    }
}

seq_t a(idx_t n)
{
    static uint32_t primes[100];
    static int np = 0;
    if (np == 0) {
        uint32_t c = 2;
        while (np < 100) {
            bool ok = true;
            for (int j = 0; j < np && primes[j] * primes[j] <= c; j++)
                if (c % primes[j] == 0) { ok = false; break; }
            if (ok) primes[np++] = c;
            c++;
        }
    }

    int e1[A340365_MAXI], e2[A340365_MAXI];
    for (int i = 0; i < A340365_MAXI; i++) { e1[i] = 0; e2[i] = 0; }

    uint64_t N = (uint64_t)n;
    A340365_add(N, e1);

    uint64_t m = N;
    for (uint64_t d = 2; d <= m / d; d++) {
        if (m % d == 0) {
            uint64_t pe = 1;
            while (m % d == 0) { m /= d; pe *= d; }
            A340365_add(pe, e2);
        }
    }
    if (m > 1) A340365_add(m, e2);

    seq_t result = 1;
    for (int i = 1; i < A340365_MAXI; i++) {
        int d = e1[i] - (e1[i] < e2[i] ? e1[i] : e2[i]);
        for (int k = 0; k < d; k++)
            result *= (seq_t)primes[i - 1];
    }
    return result;
}
