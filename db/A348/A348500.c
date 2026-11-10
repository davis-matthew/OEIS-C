/*
 * A348500  a(n) = A348494(A276086(n)).
 * https://oeis.org/A348500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A348494(A276086(n)) = gcd(D,P), where m = A276086(n) =
 *            prod p_i^e_i, D = sum e_i * (rad(m)/p_i), P = prod ((e_i+1)*p_i
 *            - e_i). The value m is never formed
 * Range:     u8 n <= 26, u16 n <= 3794, u32 n <= 11550, u64 n <= 11550,
 *            u128 n <= 11550, big any n
 * Validated: standard, pass, 122 terms through n = 8893 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A348500_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static uint64_t A348500_next_prime(uint64_t p)
{
    uint64_t c = p + 1;
    for (;; ++c) {
        bool ok = (c >= 2);
        for (uint64_t d = 2; d * d <= c; ++d) {
            if (c % d == 0) { ok = false; break; }
        }
        if (ok) return c;
    }
}

seq_t a(idx_t n)
{
    uint64_t primes[80];
    uint64_t exps[80];
    int cnt = 0;
    uint64_t p = 2;
    idx_t rem = n;

    /* primorial-base digits: digit k has radix prime(k+1) */
    while (rem > 0) {
        uint64_t d = (uint64_t)(rem % p);
        rem /= p;
        if (d > 0) {
            primes[cnt] = p;
            exps[cnt] = d;
            ++cnt;
        }
        p = A348500_next_prime(p);
    }

    seq_t D = 0;
    seq_t P = 1;
    for (int i = 0; i < cnt; ++i) {
        seq_t prodOthers = 1;
        for (int j = 0; j < cnt; ++j) {
            if (j != i) prodOthers *= (seq_t)primes[j];
        }
        D += (seq_t)exps[i] * prodOthers;
        P *= ((seq_t)exps[i] + 1) * (seq_t)primes[i] - (seq_t)exps[i];
    }

    return A348500_gcd(D, P);
}
