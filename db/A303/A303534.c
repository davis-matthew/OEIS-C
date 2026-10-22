/*
 * A303534  Amount by which n exceeds the largest binary palindrome less than
 *          or equal to n.
 * https://oeis.org/A303534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base, look
 * Formula:   a(n) = n - largest binary palindrome ≤ n (OEIS A303534)
 * Range:     u8 n <= 9098, u16 n <= 9098, u32 n <= 9098, u64 n <= 9098,
 *            u128 n <= 9098, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Reverse the lowest 'len' bits of 'x'.  'len' ≤ 64. */
static inline uint64_t reverse_bits(uint64_t x, int len)
{
    uint64_t rev = 0;
    for (int i = 0; i < len; ++i) {
        rev = (rev << 1) | (x & 1ULL);
        x >>= 1;
    }
    return rev;
}

/* Build the binary palindrome of length 'bits' whose left half
   (including the middle bit when 'bits' is odd) is given by 'left'.
   The left half has 'half' bits where half = (bits+1)/2. */
static inline uint64_t build_pal(uint64_t left, int bits)
{
    int half = (bits + 1) / 2;
    if (bits & 1) {            /* odd length */
        return (left << (half - 1))
             | reverse_bits(left >> 1, half - 1);
    } else {                   /* even length */
        return (left << half)
             | reverse_bits(left, half);
    }
}

/* Largest binary palindrome ≤ n.  Assumes n is uint64_t and n > 0. */
static inline uint64_t largest_binpal(uint64_t n)
{
    /* Determine bit length 'L' of 'n'. */
    int L = 0;
    uint64_t t = n;
    while (t) {
        ++L;
        t >>= 1;
    }

    int half = (L + 1) / 2;
    uint64_t left = n >> (L - half);

    uint64_t pal = build_pal(left, L);

    if (pal <= n) {
        return pal;
    }

    /* Candidate from decreasing left half by one (if possible). */
    uint64_t cand1 = 0;
    if (left > 0) {
        cand1 = build_pal(left - 1, L);
    }

    /* Candidate from all‑ones palindrome of length L-1. */
    uint64_t cand2 = 0;
    if (L > 1) {
        cand2 = ((uint64_t)1 << (L - 1)) - 1;
    }

    /* Choose the largest candidate that is ≤ n. */
    uint64_t best = 0;
    if (cand1 <= n && cand1 > best) best = cand1;
    if (cand2 <= n && cand2 > best) best = cand2;

    return best;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t pal = largest_binpal((uint64_t)n);
    return (seq_t)n - (seq_t)pal;
}
