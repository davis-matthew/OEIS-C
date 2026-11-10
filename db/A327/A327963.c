/*
 * A327963  If A327928(n) > 0, a(n) = A276086(n), otherwise a(n) = A003415(n).
 * https://oeis.org/A327963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A276086(n) if A327928(n) > 0 (some prime p has p^p | n'),
 *            else a(n) = A003415(n) = n' (arithmetic derivative)
 * Range:     u8 n <= 19, u16 n <= 86, u32 n <= 619, u64 n <= 2310,
 *            u128 n <= 2310, big any n
 * Validated: standard, pass, 102 terms through n = 2276 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A327963_isprime_small(unsigned p)
{
    if (p < 2) return false;
    for (unsigned d = 2; d * d <= p; ++d)
        if (p % d == 0) return false;
    return true;
}

/* arithmetic derivative of n */
static seq_t A327963_deriv(seq_t n)
{
    if (n <= 1) return 0;
    seq_t result = 0;
    seq_t rem = n;
    for (seq_t p = 2; p * p <= rem; p++) {
        int e = 0;
        while (rem % p == 0) {
            e++;
            rem /= p;
        }
        if (e > 0) result += (n / p) * (seq_t)e;
    }
    if (rem > 1) result += n / rem;
    return result;
}

/* A327928(n) > 0 ? : exists prime p with p^p | d. p is bounded by bit length of d. */
static bool A327963_has_pp(seq_t d)
{
    if (d <= 1) return false;
    for (unsigned p = 2; p <= 130; ++p) {
        if (!A327963_isprime_small(p)) continue;
        seq_t temp = d;
        unsigned e = 0;
        while (e < p && temp % (seq_t)p == 0) {
            temp /= (seq_t)p;
            ++e;
        }
        if (e >= p) return true;
    }
    return false;
}

/* A276086: primorial base digits become exponents of successive primes */
static seq_t A327963_A276086(seq_t n)
{
    seq_t result = 1;
    unsigned p = 2;
    while (n > 0) {
        seq_t digit = n % (seq_t)p;
        n /= (seq_t)p;
        for (seq_t i = 0; i < digit; ++i)
            result *= (seq_t)p;
        do { ++p; } while (!A327963_isprime_small(p));
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t d = A327963_deriv(nn);
    if (A327963_has_pp(d))
        return A327963_A276086(nn);
    return d;
}
