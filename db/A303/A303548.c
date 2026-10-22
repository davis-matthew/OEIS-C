/*
 * A303548  For any n > 0 and h > 0, let d_h(n) be the distance from n to the
 *          nearest number with Hamming weight at most h; a(n) = Sum_{i > 0}
 *          d_i(n).
 * https://oeis.org/A303548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = Sum_{i>0} d_i(n) where d_i(n) is the distance from n * to
 *            the nearest integer whose binary Hamming weight is ≤ i
 * Range:     u8 n <= 646, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A303548 for index n >= 1. */
seq_t a(idx_t n) {
    /* Unsigned 64‑bit intermediate type for bitwise operations. */
    uint64_t nn = (uint64_t)n;
    /* Number of 1‑bits in n (popcount). */
    unsigned int h = __builtin_popcountll(nn);
    seq_t sum = 0;

    /* For each i = 1 .. h, find nearest number with popcount ≤ i. */
    for (unsigned int i = 1; i <= h; ++i) {
        /* Search downward for nearest such number. */
        uint64_t low = nn;
        while (low > 0 && (unsigned int)__builtin_popcountll(low) > i)
            --low;
        /* Search upward for nearest such number. */
        uint64_t high = nn;
        while ((unsigned int)__builtin_popcountll(high) > i)
            ++high;

        /* Compute distances to n. */
        seq_t dist_low  = (seq_t)(nn - low);
        seq_t dist_high = (seq_t)(high - nn);
        sum += (dist_low < dist_high) ? dist_low : dist_high;
    }
    return sum;
}
