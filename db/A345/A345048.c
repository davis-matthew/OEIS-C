/*
 * A345048  a(n) = A342001(n) * A051709(n).
 * https://oeis.org/A345048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A342001(n) * A051709(n), where A342001(n) =
 *            rad(n)*Sum_{p^v||n} v/p and A051709(n) = sigma(n) + phi(n) - 2n
 * Range:     u8 n <= 29, u16 n <= 329, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 97 terms through n = 9636 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static seq_t A345048_a342001(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;
    seq_t rad = (seq_t)1;
    seq_t result = (seq_t)0;
    idx_t t = n;
    /* first pass: radical */
    for (idx_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            while (t % p == 0) t /= p;
            rad *= (seq_t)p;
        }
    }
    if (t > 1) rad *= (seq_t)t;

    /* second pass: sum v * rad/p */
    t = n;
    for (idx_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            idx_t e = 0;
            while (t % p == 0) { t /= p; ++e; }
            result += (seq_t)e * (rad / (seq_t)p);
        }
    }
    if (t > 1) result += rad / (seq_t)t;
    return result;
}

static seq_t A345048_a051709(idx_t n)
{
    /* sigma and phi from the factorization, kept in seq_t */
    seq_t sigma = (seq_t)1;
    seq_t phi = (seq_t)1;
    idx_t t = n;
    for (idx_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            seq_t pk = (seq_t)1;
            seq_t s = (seq_t)1;
            seq_t ph = (seq_t)1;
            while (t % p == 0) {
                t /= p;
                ph = (pk == (seq_t)1) ? (seq_t)(p - 1) : ph * (seq_t)p;
                pk *= (seq_t)p;
                s += pk;
            }
            sigma *= s;
            phi *= ph;
        }
    }
    if (t > 1) {
        sigma *= (seq_t)t + 1;
        phi *= (seq_t)t - 1;
    }
    /* sigma + phi - 2n >= 0 always, so no unsigned underflow */
    return sigma + phi - (seq_t)2 * (seq_t)n;
}

seq_t a(idx_t n)
{
    return A345048_a342001(n) * A345048_a051709(n);
}
