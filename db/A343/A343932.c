/*
 * A343932  a(n) = (Sum_{k=1..n} k^k) mod n.
 * https://oeis.org/A343932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (Sum_{k=1..n} k^k) mod n
 * Range:     u8 n <= 266, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Modular exponentiation for seq_t. */
static seq_t mod_pow_seq(seq_t base, idx_t exp, seq_t mod) {
    seq_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) {
            result = (result * base) % mod;
        }
        exp >>= 1;
        if (exp)
            base = (base * base) % mod;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;
    seq_t nm = (seq_t)n;
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t term = mod_pow_seq((seq_t)k, k, nm);
        sum += term;
        if (sum >= nm)
            sum -= nm;
    }
    return sum;
}
