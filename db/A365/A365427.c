/*
 * A365427  a(n) = A336467(A163511(n)).
 * https://oeis.org/A365427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A336467(A163511(n)). A163511(n) = p(r) * Product_{i=1..r}
 *            p(i)^e_i, where r = A000120(n) and e_i are the gaps between
 *            consecutive 1-bits of n. A336467 is fully multiplicative with
 *            a(2) = 1 and a(p) = A000265(p+1) for odd primes p, so a(n) =
 *            Product_{i=2..r} A000265(p(i)+1)^(e_i + [i==r]),...
 * Range:     u8 n <= 130, u16 n <= 1054, u32 n <= 16385, u64 n <= 16385,
 *            u128 n <= 16385, big any n
 * Validated: standard, pass, 117 terms through n = 9853 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    static uint32_t primes[64];
    static int init = 0;
    if (!init) {
        int c = 0;
        uint32_t x = 2;
        while (c < 64) {
            bool ok = true;
            for (int j = 0; j < c; ++j) {
                if ((uint64_t)primes[j] * primes[j] > x) break;
                if (x % primes[j] == 0) { ok = false; break; }
            }
            if (ok) primes[c++] = x;
            ++x;
        }
        init = 1;
    }

    unsigned pos[64];
    int cnt = 0;
    unsigned bitpos = 1;
    idx_t tmp = n;
    while (tmp) {
        if (tmp & 1) pos[cnt++] = bitpos;
        ++bitpos;
        tmp >>= 1;
    }

    seq_t result = 1;
    unsigned prev = 0;
    for (int i = 0; i < cnt; ++i) {
        unsigned e = pos[i] - prev - 1;
        if (i == cnt - 1) e += 1;
        prev = pos[i];
        uint32_t p = primes[i];
        if (p == 2) continue;
        uint32_t f = p + 1;
        while ((f & 1) == 0) f >>= 1;
        for (unsigned k = 0; k < e; ++k) result *= (seq_t)f;
    }
    return result;
}
