/*
 * A285316  Numbers k for which A019565(k) > k.
 * https://oeis.org/A285316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 0 with A019565(k) > k, A019565(k) = Product_{i
 *            in bits of k} prime(i+1); found by scan
 * Range:     u8 n <= 243, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9476 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t A285316_primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

/* true iff A019565(k) > k, without overflow */
static int A285316_greater(uint64_t k)
{
    uint64_t res = 1;
    unsigned bit = 0;
    uint64_t m = k;
    while (m != 0) {
        if (m & 1ULL) {
            uint64_t p = A285316_primes[bit];
            if (res > k / p) return 1;   /* res*p > k */
            res *= p;
        }
        m >>= 1;
        ++bit;
    }
    return res > k;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t k = 0;
    for (;; ++k) {
        if (A285316_greater(k)) {
            ++count;
            if (count == n) return (seq_t)k;
        }
    }
}
