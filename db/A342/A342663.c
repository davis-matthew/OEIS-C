/*
 * A342663  Numerator of ratio A342661(n)/A342662(n): a(n) = A342661(n) /
 *          gcd(A342661(n), A342662(n)).
 * https://oeis.org/A342663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A342661(n)/gcd(A342661(n), A342662(n)), with A342661(n) =
 *            n*sigma(A064989(n)), A342662(n) = sigma(n)*A064989(n);
 *            A064989(2^e)=1, A064989(p^e)=prevprime(p)^e for odd p
 * Range:     u8 n <= 18, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 74 (2026-11-10)
 * Helpers:   factor_u64, gcd, ipow, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/ipow.h"
#include "../utils/is_prime.h"

static idx_t A342663_prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
}

seq_t a(idx_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    if (n <= 1) return 1;
    factor_u64(n, primes, exps, &cnt);

    seq_t x = 1; /* A342661(n) */
    seq_t y = 1; /* A342662(n) */

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p = (seq_t)primes[i];
        seq_t e = (seq_t)exps[i];
        seq_t pe = ipow(p, e);
        seq_t sig_p = (ipow(p, e + 1) - 1) / (p - 1);
        seq_t qe = 1;
        seq_t sig_q = 1;
        if (primes[i] != 2)
        {
            seq_t q = (seq_t)A342663_prev_prime((idx_t)primes[i]);
            qe = ipow(q, e);
            sig_q = (ipow(q, e + 1) - 1) / (q - 1);
        }
        x *= pe * sig_q;
        y *= sig_p * qe;
    }

    seq_t g = gcd(x, y);
    return x / g;
}
