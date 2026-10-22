/*
 * A344603  Triangular numbers whose Hamming weight is also triangular.
 * https://oeis.org/A344603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   The nth element is the nth triangular number whose binary
 *            Hamming weight is also a triangular number. a(n) = T_k where
 *            k*(k+1)/2 is triangular and popcount(T_k) is triangular
 * Range:     u8 n <= 6, u16 n <= 80, u32 n <= 9970, u64 n <= 9970,
 *            u128 n <= 9970, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer square root for small unsigned integers (uses only integer ops). */
static inline size_t isqrt_uint64(uint64_t n) {
    size_t r = 0;
    while ((uint64_t)(r + 1u) * (uint64_t)(r + 1u) <= n) ++r;
    return r;
}

/* Test whether an unsigned integer w is triangular.
   w is triangular iff 8*w+1 is a perfect square and (sqrt-1) is even. */
static inline bool is_triangular_uint(size_t w) {
    uint64_t val = 8 * (uint64_t)w + 1;
    size_t s   = isqrt_uint64(val);
    return (s * s == val) && ((s & 1u) == 1u); /* sqrt odd => (sqrt-1) even */
}

/* Compute the Hamming weight (popcount) of a seq_t value. */
static inline size_t popcount_seq_t(seq_t x) {
    size_t cnt = 0;
    const seq_t one = (seq_t)1;
    while (x) {
        if ((x & one) != (seq_t)0) ++cnt;
        x >>= 1;
    }
    return cnt;
}

/* Main sequence generator. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for offset 1, but safe guard */

    idx_t k = 0;
    size_t found = 0;

    for (;;) {
        /* triangular number T_k = k*(k+1)/2 in seq_t */
        seq_t tk = (seq_t)k * ((seq_t)k + (seq_t)1) / (seq_t)2;

        /* check if popcount of tk is triangular */
        if (is_triangular_uint(popcount_seq_t(tk))) {
            ++found;
            if (found == (size_t)n) return tk;
        }
        ++k;
    }
}
