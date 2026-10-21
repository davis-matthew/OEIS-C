/*
 * A275811  Number of nonzero digits on a maximally occupied slope of
 *          factorial base representation of n: a(n) = A051903(A275734(n)).
 *          See comments for the definition.
 * https://oeis.org/A275811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of occurrences of the most common element in the
 *            multiset * { i - d | i = position from the right (1‑based) of a
 *            nonzero digit d in the factorial * base expansion of n }
 * Range:     u8 n <= 9869, u16 n <= 9869, u32 n <= 9869, u64 n <= 9869,
 *            u128 n <= 9869, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* no non‑zero digits */

    /* factorials up to 20! fit in uint64_t (20! = 2 432 902 008 176 640 000) */
    uint64_t fac[21];
    fac[0] = 1;
    unsigned int maxpos = 0;
    for (unsigned int pos = 1; pos <= 20; ++pos) {
        fac[pos] = fac[pos - 1] * (uint64_t)pos;
        if (fac[pos] <= (uint64_t)n) {
            maxpos = pos;
        } else {
            break;
        }
    }

    /* histogram of differences diff = i - d  (diff ≤ maxpos‑1) */
    unsigned int hist[64] = {0};    /* 64 elements enough for all possible diff values */
    uint64_t rem = (uint64_t)n;
    for (int p = (int)maxpos; p >= 1; --p) {
        unsigned int digit = (unsigned int)(rem / fac[p]); /* digit ≤ p */
        rem %= fac[p];
        if (digit != 0) {
            unsigned int diff = p - digit;    /* 0 ≤ diff ≤ p‑1 */
            hist[diff]++;
        }
    }

    unsigned int maxfreq = 0;
    for (unsigned int i = 0; i < maxpos; ++i) {
        if (hist[i] > maxfreq) maxfreq = hist[i];
    }
    return (seq_t)maxfreq;
}
