/*
 * A298847  Lexicographically earliest sequence of distinct positive terms
 *          such that, for any n > 0, the number of ones in the binary
 *          expansion of n equals one plus the number of zeros in the binary
 *          expansion of a(n).
 * https://oeis.org/A298847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(n) is the lexicographically earliest permutation of the positive integers
 * such that for every n > 0,
 *
 *     popcount(n) = 1 + zeros(a(n))
 *
 * where popcount(x) is the number of ones in the binary expansion of x and
 * zeros(x) is the number of zeros in that expansion.  On each block
 * [2^k, 2^(k+1)-1] (numbers with the same bit-length k+1) the elements are
 * paired by complementary popcounts: a number with popcount p is paired with
 * the element of the same rank in the group of numbers with popcount
 * (k + 2 - p).  The sequence is self‑inverse and has fixed points given by
 * the numbers whose popcount equals the block bit-length minus one.
 *
 * The implementation below follows this description directly and works
 * for any build‑defined types seq_t and idx_t.  The function is O(k + m)
 * per call, where k=⌊log₂n⌋ and m is the size of the block (≤ 2^(k+1)),
 * which easily meets the 2 s limit for n up to about 10 000.
 */

#include <stdint.h>

/* Portable popcount for idx_t using builtin when available */
static inline unsigned int popcount_u64(idx_t x) {
#if defined(__GNUC__) || defined(__clang__)
    return (unsigned int)__builtin_popcountll(x);
#else
    unsigned int c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
#endif
}

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined for offset 1; harmless */

    /* Determine block: find k with 2^k <= n < 2^(k+1) */
    idx_t power = 1;
    idx_t k = 0;
    while ((power << 1) <= n) {
        power <<= 1;
        ++k;
    }
    idx_t base = power;              /* 2^k */
    unsigned int L = (unsigned int)(k + 1);   /* bit-length of numbers in this block */

    unsigned int p = popcount_u64(n);            /* popcount of n   */
    unsigned int q = L - p + 1;                  /* complementary popcount */

    /* Rank of n within its popcount group inside the block */
    idx_t rank = 0;
    for (idx_t m = base; m < n; ++m) {
        if (popcount_u64(m) == p)
            ++rank;
    }

    /* Find the element with the same rank in the q-popcount group */
    idx_t counter = 0;
    idx_t block_end = base << 1;     /* 2^(k+1) – exclusive */
    for (idx_t m = base; m < block_end; ++m) {
        if (popcount_u64(m) == q) {
            if (counter == rank)
                return (seq_t)m;
            ++counter;
        }
    }

    /* Should never happen for valid inputs */
    return 0;
}
