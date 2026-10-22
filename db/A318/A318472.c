/*
 * A318472  Multiplicative with a(p^e) = 2^A000045(e).
 * https://oeis.org/A318472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 2^(∑_{p^e‖n} Fib(e)), where Fib(e) is the e‑th Fibonacci
 *            number (Fib(0)=0, Fib(1)=1)
 * Range:     u8 n <= 63, u16 n <= 255, u32 n <= 511, u64 n <= 2047,
 *            u128 n <= 4095, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined below the OEIS offset; returning 0 is harmless. */
        return 0;
    }

    /* Fibonacci numbers up to exponent 64 (sufficient for any trial
       factorisation of uint64_t values).  They are pre‑computed once. */
    static seq_t fib[65];
    static int fib_initialized = 0;
    if (!fib_initialized) {
        fib[0] = 0;
        fib[1] = 1;
        for (int i = 2; i < 65; ++i) {
            fib[i] = fib[i - 1] + fib[i - 2];
        }
        fib_initialized = 1;
    }

    /* Sum of Fib(e) over all prime exponents e in the factorisation of n. */
    seq_t expSum = 0;
    uint64_t temp = (uint64_t)n;

    /* Trial division factorisation.  This is fast enough for n up to about
       10 000 (the intended usage limit). */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            expSum += fib[(idx_t)e];
        }
    }

    /* If a prime factor larger than sqrt(n) remains, it occurs to the
       first power. */
    if (temp > 1) {
        expSum += fib[1];      /* e = 1 */
    }

    /* a(n) = 2^expSum.  pow2 is provided by the utilities. */
    return pow2((idx_t)expSum);
}
