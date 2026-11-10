/*
 * A373148  a(n) = A276085(n) mod A003415(n), where A003415 is the arithmetic
 *          derivative and A276085 is the primorial base log-function.
 * https://oeis.org/A373148
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A276085(n) mod A003415(n), where A276085(n) = Sum_{p^e ||
 *            n} e * (product of primes < p) and A003415(n) = Sum_{p^e || n} e
 *            * n/p; A276085 is reduced modulo A003415(n) as it is built, so
 *            intermediate values stay small
 * Range:     u8 n <= 233, u16 n <= 20991, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 113 terms through n = 9839 (2026-11-10)
 * Helpers:   mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mulmod.h"

static bool A373148_isprime(uint64_t q)
{
    if (q < 2) return false;
    if (q < 4) return true;
    if (q % 2 == 0) return false;
    for (uint64_t d = 3; d * d <= q; d += 2)
        if (q % d == 0) return false;
    return true;
}

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    uint64_t m = N;
    uint64_t facs[64];
    uint64_t exps[64];
    size_t cnt = 0;

    for (uint64_t d = 2; d * d <= m; d++) {
        if (m % d == 0) {
            uint64_t e = 0;
            while (m % d == 0) { m /= d; e++; }
            facs[cnt] = d; exps[cnt] = e; cnt++;
        }
    }
    if (m > 1) { facs[cnt] = m; exps[cnt] = 1; cnt++; }

    /* arithmetic derivative */
    uint64_t D = 0;
    for (size_t i = 0; i < cnt; i++)
        D += (N / facs[i]) * exps[i];
    if (D <= 1)
        return (seq_t)0;

    /* primorial base log value modulo D */
    uint64_t A = 0;
    uint64_t prim = 1 % D;   /* product of primes < q, mod D */
    size_t k = 0;
    for (uint64_t q = 2; k < cnt; q++) {
        if (!A373148_isprime(q))
            continue;
        if (q == facs[k]) {
            A = (A + mulmod(exps[k] % D, prim, D)) % D;
            k++;
        }
        prim = mulmod(prim, q % D, D);
    }

    return (seq_t)A;
}
