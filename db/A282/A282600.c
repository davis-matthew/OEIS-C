/*
 * A282600  a(n) = Sum_(k=1..phi(n)) floor(d_k/2) where d_k are the totatives
 *          of n.
 * https://oeis.org/A282600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = φ(n)*(n-1)/4 if n is odd, a(n) = φ(n)*(n-2)/4 if n is
 *            even where φ(n) is Euler's totient function. This follows from
 *            a(n) = ⌊∑_{k∈R(n)} k / 2⌋, where R(n) is the reduced residue
 *            system mod n. For n>1, ∑ k = n·φ(n)/2, so a(n) simplifies to the
 *            expressions above. The implementation evaluates...
 * Range:     u8 n <= 36, u16 n <= 520, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute Euler's totient function φ(n) for n≥1.  The result fits
   into a 64‑bit integer for all n considered by the problem (n≤10000). */
static uint64_t _phi_uint64(uint64_t n) {
    uint64_t result = n;
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            /* p is a distinct prime factor */
            while (tmp % p == 0) {
                tmp /= p;
            }
            result -= result / p;
        }
    }
    if (tmp > 1) {                /* remaining prime factor */
        result -= result / tmp;
    }
    return result;
}

/* Public sequence function.  The type names seq_t and idx_t are
   supplied by the build system. */
seq_t a(idx_t n) {
    /* Convert to a mutable integer for the φ calculation. */
    uint64_t n_val = (uint64_t)n;
    uint64_t phi_val = _phi_uint64(n_val);

    /* Depending on parity of n, use the appropriate multiplier. */
    uint64_t multiplier = (n_val & 1ULL) ? (n_val - 1ULL) : (n_val - 2ULL);

    /* Compute φ(n) * multiplier / 4 using 64‑bit arithmetic. */
    uint64_t product = phi_val * multiplier;
    uint64_t a_val = product / 4ULL;

    return (seq_t)a_val;
}
