/*
 * A354205  a(n) = sigma(A354202(n)), where A354202 is fully multiplicative
 *          with a(p) = A354200(A000720(p)).
 * https://oeis.org/A354205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   multiplicative, a(p^e) = (q^(e+1)-1)/(q-1) = 1+q+...+q^e, where
 *            q = next prime after p with the same residue mod 4 (q = 5 if p =
 *            2); a(n) = sigma(A354202(n))
 * Range:     u8 n <= 15, u16 n <= 127, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 7998 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A354205_isprime(uint64_t m)
{
    if (m < 2) return false;
    if (m % 2 == 0) return m == 2;
    for (uint64_t d = 3; d * d <= m; d += 2)
        if (m % d == 0) return false;
    return true;
}

static uint64_t A354205_map(uint64_t p)
{
    if (p == 2) return 5;
    uint64_t q = p + 4;
    while (!A354205_isprime(q)) q += 4;
    return q;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p;
    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            uint64_t q = A354205_map(p);
            seq_t term = 1, pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)q;
                term += pw;
            }
            result *= term;
        }
    }
    if (m > 1) {
        uint64_t q = A354205_map(m);
        result *= ((seq_t)q + 1);
    }
    return result;
}
