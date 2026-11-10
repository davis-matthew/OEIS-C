/*
 * A358218  Number of prime factors (with multiplicity) in A328478(n).
 * https://oeis.org/A358218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = bigomega(A328478(n)), where A328478(n) is the fixed point
 *            of repeatedly dividing n by its largest dividing primorial
 * Range:     u8 n <= 1832, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 136 terms through n = 8888 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static seq_t A328478_a(idx_t n)
{
    static bool init = false;
    static uint64_t primes[300];
    static size_t cnt = 0;
    if (!init) {
        uint64_t c = 2;
        while (cnt < 300) {
            bool ok = true;
            for (size_t i = 0; i < cnt && primes[i] * primes[i] <= c; ++i)
                if (c % primes[i] == 0) { ok = false; break; }
            if (ok) primes[cnt++] = c;
            ++c;
        }
        init = true;
    }
    seq_t cur = (seq_t)n;
    if (cur == 0) return cur;
    while (1) {
        seq_t largest = 1;
        seq_t primorial = 1;
        for (size_t i = 0; i < cnt; ++i) {
            if (primorial > cur / (seq_t)primes[i]) break;
            primorial = primorial * (seq_t)primes[i];
            if (cur % primorial == 0) largest = primorial;
        }
        if (largest == 1) break;
        cur = cur / largest;
    }
    return cur;
}

seq_t a(idx_t n)
{
    seq_t t = A328478_a(n);
    seq_t count = 0;
    while (t % 2 == 0 && t > 1) { count++; t /= 2; }
    for (seq_t i = 3; i <= t / i; i += 2) {
        while (t % i == 0) { count++; t /= i; }
    }
    if (t > 1) count++;
    return count;
}
