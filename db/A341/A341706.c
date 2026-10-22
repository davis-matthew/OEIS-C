/*
 * A341706  Row 2 of semigroup multiplication table shown in A341317 and
 *          A341318.
 * https://oeis.org/A341706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(0) = 0; for n>0 let n1 = n-1; find the smallest i such
 *            that i*(i+1)/2 > n1; let t_prev = (i-1)*i/2; // number of terms
 *            before block i let k = n1 - t_prev; // offset inside block i
 *            start_i = 2 + (i-1)*(2*i+1); // first number of block i a(n) =
 *            start_i + k;
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline __uint128_t triangular(uint64_t x) {
    return (__uint128_t)x * (x + 1) / 2;
}

static inline uint64_t block_index(idx_t n1) {
    /* Find smallest i with triangular(i) > n1 */
    uint64_t lo = 0, hi = 1;
    while (triangular(hi) <= (__uint128_t)n1) {
        lo = hi;
        hi <<= 1;        /* hi *= 2; safe for uint64_t */
    }
    while (lo + 1 < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (triangular(mid) > (__uint128_t)n1)
            hi = mid;
        else
            lo = mid;
    }
    return hi;
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;

    idx_t n1 = n - 1;                // shift indices to start after the zero element

    uint64_t i = block_index(n1);    // block index (1‑based)

    __uint128_t t_prev = triangular(i - 1);   // (i-1)*i/2
    idx_t k = (idx_t)(n1 - t_prev);          // offset inside block

    __uint128_t start = 2 + (__uint128_t)(i - 1) * (2 * i + 1);
    __uint128_t value = start + k;

    return (seq_t)value;
}
