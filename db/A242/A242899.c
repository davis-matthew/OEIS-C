/*
 * A242899  Least number k > 1 such that (n^k+k^n)/(k+n) is an integer.
 * https://oeis.org/A242899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = smallest k > 1 such that (n^k + k^n) is divisible by (k +
 *            n)
 * Range:     u8 n <= 506, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute a(n) for 1 <= n.
   The function is O(n * average_k) with very small constants.
   For each n it checks k from 2 up to n (a(n) <= n by OEIS remark).
   Modulo exponentiation is done with 64‑bit arithmetic because the
   modulus (k + n) is small (<= 2*n).  */
static inline seq_t powmod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1U)
            res = (res * b) % mod;
        b = (b * b) % mod;
        exp >>= 1U;
    }
    return (seq_t)res;
}

seq_t a(idx_t n)
{
    if (n <= 1) {
        /* a(1) = 2 by definition */
        return (seq_t)2;
    }

    /* n is at most our search limit (≈10 000), so 64‑bit unsigned is safe. */
    uint64_t un = (uint64_t)n;

    for (uint64_t k = 2; k <= un; ++k) {
        uint64_t mod = un + k;

        seq_t term1 = powmod_uint64(un, k, mod);
        seq_t term2 = powmod_uint64(k, un, mod);

        /* (term1 + term2) mod mod == 0  <=>  k is a valid candidate */
        if (((term1 + term2) % mod) == 0)
            return (seq_t)k;
    }

    /* In theory we should never reach here because a(n) <= n, but
       return 0 as a sentinel just in case. */
    return (seq_t)0;
}
