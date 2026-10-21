/*
 * A280450  Smallest k > 2n+1 such that phi(k) = phi(2n+1).
 * https://oeis.org/A280450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k > 2n+1 such that phi(k) = phi(2n+1)
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using a simple trial‑division totient computation. */

#include <stdbool.h>
#include <stdint.h>

/* Local totient calculation using trial division.  Works for
   values comfortably within seq_t (the sequence is defined only
   for n ≤ about 10 000). */
static seq_t phi_seq(seq_t x)
{
    uint64_t m = (uint64_t)x;
    uint64_t result = m;

    if (m == 0) return 0;

    /* Handle factor 2 */
    if ((m & 1ULL) == 0) {
        while ((m & 1ULL) == 0) m >>= 1;
        result -= result >> 1; /* result * (1 - 1/2) */
    }

    /* Odd factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            result -= result / p;
        }
    }

    if (m > 1) result -= result / m;
    return (seq_t)result;
}

seq_t a(idx_t n)
{
    /* Offset 1 – a(1) is the first term.  For n < 1 the
       function returns 0 (undefined). */
    if (n < 1) return 0;

    /* m = 2n + 1; phi_m = phi(m) */
    seq_t m      = (seq_t)2 * (seq_t)n + (seq_t)1;
    seq_t phi_m  = phi_seq(m);

    /* Search k from m+1 up to 4n+3 inclusive. */
    seq_t upper  = (seq_t)4 * (seq_t)n + (seq_t)3;
    for (seq_t k = m + (seq_t)1; k <= upper; ++k) {
        if (phi_seq(k) == phi_m) return k;
    }

    /* By the bounds proved in OEIS, a solution always exists. */
    return 0;
}
