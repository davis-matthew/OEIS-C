/*
 * A387070  Remove every digit that appears in n from the decimal
 *          representation of n^2. If no digits remain, set a(n) = 0.
 * https://oeis.org/A387070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = remove every digit that appears in n from the decimal
 *            representation of n^2; return 0 if no digits remain
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    bool forbidden[10] = {false};

    /* Identify digits to remove (digits of n). */
    idx_t tmp = n;
    if (tmp == 0) {
        forbidden[0] = true;
    } else {
        while (tmp) {
            forbidden[tmp % 10] = true;
            tmp /= 10;
        }
    }

    /* Compute n^2. */
    seq_t sq = (seq_t)n * (seq_t)n;

    /* Store the remaining digits in reverse order. */
    uint8_t rev[200];
    unsigned int revlen = 0;

    do {
        uint8_t d = (uint8_t)(sq % 10);
        if (!forbidden[d]) {
            /* revlen is bounded by 200; this is sufficient for all
               practical inputs of the test harness (up to ~128‑bit
               values, which have <40 decimal digits). */
            rev[revlen++] = d;
        }
        sq /= 10;
    } while (sq != 0);

    if (revlen == 0) return 0;

    /* Reconstruct the result from most significant to least. */
    seq_t result = 0;
    for (unsigned int i = revlen; i > 0; --i) {
        result = result * 10 + rev[i - 1];
    }

    return result;
}
