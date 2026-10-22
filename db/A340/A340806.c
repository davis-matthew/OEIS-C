/*
 * A340806  a(n) = Sum_{k=1..n-1} (k^n mod n).
 * https://oeis.org/A340806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n-1} (k^n mod n)
 * Range:     u8 n <= 25, u16 n <= 364, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute base^exp mod mod using 64‑bit arithmetic with 128‑bit multiplication
   to avoid overflow.  Result is returned as idx_t so it is safe for all n
   up to the size of idx_t. */
static inline idx_t mod_pow_idx(idx_t base, idx_t exp, idx_t mod)
{
    if (mod == 1) return 0;
    idx_t result = 1 % mod;
    idx_t b = base % mod;

    while (exp > 0) {
        if (exp & 1) {
            unsigned __int128 prod = (unsigned __int128)result * b;
            result = (idx_t)(prod % mod);
        }
        exp >>= 1;
        if (exp) {
            unsigned __int128 prod = (unsigned __int128)b * b;
            b = (idx_t)(prod % mod);
        }
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;   /* a(1) = 0, and a(0) undefined but handled */

    idx_t sum = 0;
    for (idx_t k = 1; k < n; ++k) {
        idx_t term = mod_pow_idx(k, n, n);   /* k^n mod n */
        sum += term;
    }
    return (seq_t)sum;
}
