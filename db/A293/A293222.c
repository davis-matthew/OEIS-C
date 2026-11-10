/*
 * A293222  a(n) = Product_{d|n, d<n} A019565(A289814(d)); a product obtained
 *          from the 2-digits present in ternary expansions of proper divisors
 *          of n.
 * https://oeis.org/A293222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Product_{d|n, d<n} A019565(A289814(d)), where A289814(d)
 *            is the binary mask of the 2-digits in the ternary expansion of
 *            d, and A019565(m) = Product_{k in bits of m} prime(k+1)
 * Range:     u8 n <= 47, u16 n <= 119, u32 n <= 311, u64 n <= 959,
 *            u128 n <= 3359, big any n
 * Validated: standard, pass, 115 terms through n = 6364 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t A293222_primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

/* A019565(A289814(d)) computed directly from the ternary digits of d. */
static seq_t A293222_term(idx_t d)
{
    seq_t r = 1;
    int pos = 0;
    while (d > 0) {
        if (d % 3 == 2) r *= (seq_t)A293222_primes[pos];
        d /= 3;
        pos++;
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 1; i * i <= n; i++) {
        if (n % i != 0) continue;
        idx_t j = n / i;
        if (i < n) res *= A293222_term(i);
        if (j != i && j < n) res *= A293222_term(j);
    }
    return res;
}
