/*
 * A374469  The odd part of gcd(n, A163511(n)).
 * https://oeis.org/A374469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000265(gcd(n, A163511(n))), where A163511(n) = p(r) *
 *            Product_{m=1..r} p(m)^A163510(n,m) and r = A000120(n); for each
 *            odd prime p(m) (m >= 2), its exponent in a(n) is min(v_p(n),
 *            exponent of p(m) in A163511(n)); a(0) = 1
 * Range:     u8 n <= 428, u16 n <= 70804, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 122 terms through n = 9473 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static void A374469_primes(uint32_t *pr, int cnt)
{
    int k = 0;
    uint32_t c = 2;
    while (k < cnt) {
        bool ok = true;
        for (int j = 0; j < k && pr[j] * pr[j] <= c; j++) {
            if (c % pr[j] == 0) { ok = false; break; }
        }
        if (ok) pr[k++] = c;
        c++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    uint32_t pr[64];
    A374469_primes(pr, 64);

    int cnt = 0;
    idx_t tmp = n;
    while (tmp) { cnt += (int)(tmp & 1); tmp >>= 1; }

    seq_t result = 1;
    int i = 0;
    int prev = 0;
    int bitpos = 1;
    tmp = n;
    while (tmp) {
        if (tmp & 1) {
            int e = bitpos - prev - 1;
            if (i == cnt - 1) e += 1;
            if (i >= 1 && e > 0) {
                idx_t p = pr[i];
                idx_t m = n;
                int v = 0;
                while (v < e && m % p == 0) { m /= p; v++; }
                for (int k = 0; k < v; k++) result *= (seq_t)p;
            }
            prev = bitpos;
            i++;
        }
        bitpos++;
        tmp >>= 1;
    }
    return result;
}
