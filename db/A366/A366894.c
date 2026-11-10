/*
 * A366894  a(n) = A336699(A163511(n)).
 * https://oeis.org/A366894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A336699(A163511(n)); sigma is computed multiplicatively
 *            from the prime exponents of A163511(n)
 * Range:     u8 n <= 32, u16 n <= 262, u32 n <= 8206, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 109 terms through n = 9773 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A366894_odd(seq_t x)
{
    if (x == 0) return x;
    while ((x & 1) == 0) x >>= 1;
    return x;
}

seq_t a(idx_t n)
{
    unsigned exps[64];
    unsigned primes[64];
    int i, np = 0;
    for (i = 0; i < 64; i++) exps[i] = 0;
    for (unsigned c = 2; np < 64; c++) {
        bool ok = true;
        for (int j = 0; j < np; j++) {
            if (primes[j] * primes[j] > c) break;
            if (c % primes[j] == 0) { ok = false; break; }
        }
        if (ok) primes[np++] = c;
    }

    int cnt = 0;
    unsigned prev = 0, bitpos = 1;
    idx_t tmp = n;
    while (tmp) {
        if (tmp & 1) {
            exps[cnt] = bitpos - prev - 1;
            prev = bitpos;
            cnt++;
        }
        bitpos++;
        tmp >>= 1;
    }
    if (cnt > 0) exps[cnt - 1]++;   /* extra factor p(r), r = popcount(n) */

    seq_t sig = 1;
    for (i = 1; i < cnt; i++) {      /* skip prime 2 (odd part) */
        if (exps[i] == 0) continue;
        seq_t p = (seq_t)primes[i];
        seq_t pw = 1, s = 1;
        for (unsigned k = 0; k < exps[i]; k++) {
            pw *= p;
            s += pw;
        }
        sig *= s;
    }
    seq_t o = A366894_odd(sig);
    return A366894_odd(o + 1);
}
