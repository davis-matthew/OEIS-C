/*
 * A353792  a(n) = A003958(sigma(n)) * A064989(sigma(n)).
 * https://oeis.org/A353792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = A003958(sigma(n)) * A064989(sigma(n)); factor s =
 *            sigma(n), a(n) = Product_{p^e || s} (p-1)^e * (prevprime(p))^e
 *            with the factor for p=2 being 1
 * Range:     u8 n <= 15, u16 n <= 143, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9376 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool a353792_is_prime(seq_t x)
{
    if (x < 2) return false;
    if (x < 4) return true;
    if (x % 2 == 0) return false;
    for (seq_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return false;
    return true;
}

static seq_t a353792_prevprime(seq_t p)
{
    seq_t q = p - 1;
    while (!a353792_is_prime(q)) --q;
    return q;
}

static seq_t a353792_sigma(idx_t n)
{
    seq_t result = 1;
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            seq_t term = 1, pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
            }
            result *= term;
        }
    }
    if (m > 1) result *= ((seq_t)m + 1);
    return result;
}

static seq_t a353792_factor_contrib(seq_t p, seq_t *rem)
{
    seq_t f = 1;
    seq_t pp = 0;
    while (*rem % p == 0) {
        *rem /= p;
        if (p == 2) {
            /* (2-1) * 1 = 1 */
        } else {
            if (pp == 0) pp = a353792_prevprime(p);
            f *= (p - 1) * pp;
        }
    }
    return f;
}

seq_t a(idx_t n)
{
    seq_t rem = a353792_sigma(n);
    seq_t result = 1;
    for (seq_t p = 2; p <= rem / p; p += (p == 2 ? 1 : 2)) {
        if (rem % p == 0)
            result *= a353792_factor_contrib(p, &rem);
    }
    if (rem > 1)
        result *= a353792_factor_contrib(rem, &rem);
    return result;
}
