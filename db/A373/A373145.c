/*
 * A373145  a(n) = gcd(A003415(n), A276085(n)), where A003415 is the
 *          arithmetic derivative and A276085 is the primorial base
 *          log-function.
 * https://oeis.org/A373145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A003415(n), A276085(n)); A276085(n) is reduced modulo
 *            A003415(n), since gcd(D, P) = gcd(D, P mod D), so the huge
 *            primorial terms never overflow
 * Range:     u8 n <= 537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9938 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static uint64_t A373145_gcd_u64(uint64_t x, uint64_t y) {
    while (y != 0) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static uint64_t A373145_mulmod(uint64_t x, uint64_t y, uint64_t m) {
    return (uint64_t)(((unsigned __int128)x * (unsigned __int128)y) % (unsigned __int128)m);
}

seq_t a(idx_t n) {
    if (n <= 1)
        return (seq_t)0;

    uint64_t facs[64];
    uint64_t exps[64];
    size_t fcnt = 0;

    uint64_t rem = (uint64_t)n;
    for (uint64_t p = 2; p * p <= rem; p++) {
        if (rem % p == 0) {
            uint64_t e = 0;
            while (rem % p == 0) {
                rem /= p;
                e++;
            }
            facs[fcnt] = p;
            exps[fcnt] = e;
            fcnt++;
        }
    }
    if (rem > 1) {
        facs[fcnt] = rem;
        exps[fcnt] = 1;
        fcnt++;
    }

    /* arithmetic derivative */
    uint64_t N = (uint64_t)n;
    uint64_t D = 0;
    for (size_t i = 0; i < fcnt; i++)
        D += (N / facs[i]) * exps[i];

    /* primorial log modulo D: walk primes up to the largest factor */
    uint64_t plog = 0;
    uint64_t prod = 1 % D;
    size_t k = 0;
    uint64_t maxp = facs[fcnt - 1];
    for (uint64_t q = 2; q <= maxp; q++) {
        bool pr = true;
        for (uint64_t d = 2; d * d <= q; d++) {
            if (q % d == 0) { pr = false; break; }
        }
        if (!pr)
            continue;
        if (k < fcnt && facs[k] == q) {
            plog = (plog + A373145_mulmod(exps[k] % D, prod, D)) % D;
            k++;
        }
        prod = A373145_mulmod(prod, q % D, D);
    }

    return (seq_t)A373145_gcd_u64(D, plog);
}
