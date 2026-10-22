/*
 * A377421  Numbers whose binary reversal is prime and unequal to the original
 *          number.
 * https://oeis.org/A377421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = numbers whose binary reversal is prime and unequal to the
 *            original number
 * Range:     u8 n <= 111, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Reverse the binary representation of a 64‑bit number */
static inline uint64_t rev_uint64(uint64_t v) {
    uint64_t rev = 0;
    while (v) {
        rev = (rev << 1) | (v & 1ULL);
        v >>= 1;
    }
    return rev;
}

/* Simple deterministic primality test for 64‑bit numbers */
static inline bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    if ((n & 1ULL) == 0) return n == 2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL)
        if (n % d == 0ULL) return false;
    return true;
}

/* Compute the n-th term of A377421 (1‑based).  */
extern seq_t a(idx_t n) {
    if (n == 0) return 0;        /* out of range, but not used */
    seq_t result = 0;
    idx_t count = 0;

    for (idx_t cand = 1; count < n; ++cand) {
        uint64_t rev = rev_uint64(cand);
        if (rev != cand && is_prime_uint64(rev)) {
            ++count;
            result = (seq_t)cand;
        }
    }
    return result;
}
