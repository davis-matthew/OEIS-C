/*
 * A361284  Number of unordered triples of self-avoiding paths whose sets of
 *          nodes are disjoint subsets of a set of n points on a circle;
 *          one-node paths are not allowed.
 * https://oeis.org/A361284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*(n-1)*(n-2)/384) * (7^(n-3) - 3*5^(n-3) + 3^(n-2) - 1)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 24, u128 n <= 44,
 *            big any n
 * Helpers:   mul_small, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include helpers needed for the arbitrary‑precision path */
#include "../utils/seq_pow.h"
#include "../utils/mul_small.h"

static inline __int128 pow128(__int128 base, idx_t exp) {
    __int128 res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= base;
    return res;
}

seq_t a(idx_t n)
{
    /* short returns for small n */
    if (n < 3) return 0;

    /* ---- path for small seq_t (<= 64 bits) ---- */
    if (sizeof(seq_t) <= 8) {
        /* Use 128‑bit intermediate calculations */
        __int128 a_ = n;
        __int128 b_ = n - 1;
        __int128 c_ = n - 2;

        __int128 pow7  = pow128(7,  n - 3);
        __int128 pow5  = pow128(5,  n - 3);
        __int128 pow3  = pow128(3,  n - 2);

        __int128 inside = pow7 - (__int128)3 * pow5 + pow3 - 1;

        __int128 prod   = a_ * b_ * c_;
        __int128 whole  = prod * inside;
        __int128 res    = whole / 384;

        return (seq_t)res;
    }

    /* ---- path for arbitrary‑precision seq_t ---- */
    seq_t a_ = (seq_t)n;
    seq_t b_ = (seq_t)(n - 1);
    seq_t c_ = (seq_t)(n - 2);

    /* Compute the powers using seq_pow (big integer arithmetic) */
    seq_t pow7 = seq_pow((seq_t)7, n - 3);
    seq_t pow5 = seq_pow((seq_t)5, n - 3);
    seq_t pow3 = seq_pow((seq_t)3, n - 2);

    /* inside = 7^(n-3) - 3*5^(n-3) + 3^(n-2) - 1 */
    seq_t inside = pow7;
    inside -= mul_small(pow5, 3);
    inside += pow3;
    inside -= 1;

    /* Compute the numerator: (n*(n-1)*(n-2)*inside) / 384 */
    seq_t prod = a_;
    prod *= b_;
    prod *= c_;
    prod *= inside;

    return prod / 384;
}
