/*
 * A372639  a(n) = (1/4) * Sum_{k=1..n} phi(10*k).
 * https://oeis.org/A372639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/4) * Sum_{k=1..n} φ(10*k) (offset 1)
 * Range:     u8 n <= 11, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute Euler's totient function for an unsigned 64‑bit integer.
   The result is returned as a seq_t value. */
static seq_t phi_uint64(uint64_t n)
{
    seq_t result = (seq_t)n;
    uint64_t t = n;

    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            result = result / (seq_t)p * ((seq_t)p - 1);
            while (t % p == 0) {
                t /= p;
            }
        }
    }
    if (t > 1) {
        result = result / (seq_t)t * ((seq_t)t - 1);
    }
    return result;
}

/* OEIS sequence A372639: a(n) = (1/4) * Sum_{k=1..n} φ(10*k) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1, harmless guard */

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* compute φ(10*k) */
        sum += phi_uint64(10 * (uint64_t)k);
    }
    return sum / 4;
}
