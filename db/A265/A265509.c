/*
 * A265509  a(n) = largest base-2 palindrome m <= 2n+1 such that every base-2
 *          digit of m is <= the corresponding digit of 2n+1; m is written in
 *          base 10.
 * https://oeis.org/A265509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the largest base‑2 palindrome m ≤ 2n+1 such that
 * every binary digit of m is ≤ the corresponding digit of 2n+1.  The
 * function works for any build where seq_t and idx_t are available
 * (integer types up to 128 bits or an arbitrary‑precision class). It
 * uses only integer arithmetic on native types, since the values for
 * n ≤ 10 000 fit comfortably in 64 bits. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 2n + 1 as seq_t, then cast to uint64_t (safe for n ≤ 10000). */
    seq_t tmp = (seq_t)n * 2 + 1;
    uint64_t ux = (uint64_t)tmp;          /* value of 2n+1 */

    /* Compute bit length of ux. */
    unsigned len = 0;
    uint64_t t = ux;
    do { ++len; t >>= 1; } while (t);

    /* half = ceil(len/2) used to iterate over the independent bits */
    unsigned half = (len + 1) / 2;
    uint64_t limit = 1ULL << half;

    /* allowed[i] == 1 if both mirrored positions of bit i in ux are 1 */
    uint8_t allowed[16] = {0};
    for (unsigned i = 0; i < half; ++i) {
        unsigned j = len - 1 - i;
        if ( ((ux >> i) & 1) && ((ux >> j) & 1) )
            allowed[i] = 1;
    }

    uint64_t best = 0;

    /* Enumerate all allowed masks for the half of the bits. */
    for (uint64_t mask = 0; mask < limit; ++mask) {
        /* skip masks that try to set a forbidden bit */
        if (mask & ~((uint64_t)0)) { }
        int ok = 1;
        for (unsigned i = 0; i < half; ++i) {
            if ( (mask >> i) & 1ULL ) {
                if (!allowed[i]) { ok = 0; break; }
            }
        }
        if (!ok) continue;

        uint64_t m = 0;
        for (unsigned i = 0; i < half; ++i) {
            if ( (mask >> i) & 1ULL ) {
                m |= 1ULL << i;
                unsigned j = len - 1 - i;
                if (i != j) m |= 1ULL << j;
            }
        }
        if (m > best) best = m;
    }

    return (seq_t)best;
}
