/*
 * A366887  Sum of the odd squarefree divisors permuted by A163511.
 * https://oeis.org/A366887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A206787(A163511(n)); A163511(n) = p(r) * Product_{i=1..r}
 *            p(i)^e_i, r = popcount(n), so a(n) = Product over odd primes p
 *            dividing it of (1+p)
 * Range:     u8 n <= 106, u16 n <= 1706, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 109 terms through n = 9848 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    unsigned primes[64];
    int pc = 0;
    for (unsigned c = 2; pc < 64; c++) {
        int ok = 1;
        for (int j = 0; j < pc && primes[j] * primes[j] <= c; j++) {
            if (c % primes[j] == 0) { ok = 0; break; }
        }
        if (ok) primes[pc++] = c;
    }

    int flag[64];
    int cnt = 0;
    idx_t prev = 0, bitpos = 1, tmp = n;
    while (tmp) {
        if (tmp & 1) {
            flag[cnt++] = (bitpos - prev - 1) > 0;
            prev = bitpos;
        }
        bitpos++;
        tmp >>= 1;
    }
    flag[cnt - 1] = 1; /* the leading factor p(r) */

    seq_t result = 1;
    for (int i = 0; i < cnt; i++) {
        if (flag[i] && primes[i] != 2)
            result *= (seq_t)(primes[i] + 1);
    }
    return result;
}
