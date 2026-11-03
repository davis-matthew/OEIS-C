/*
 * A372120  Numbers k such that the k-th composition in standard order is
 *          biquanimous.
 * https://oeis.org/A372120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest nonnegative integer k such that * the
 *            k-th composition in standard order * (graded
 *            reverse‑lexicographic, A066099) is biquanimous
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Return true if the k‑th composition (standard order) is biquanimous. */
static bool is_biquanimous(uint64_t k)
{
    uint64_t prev = 0;          /* previous position of a 1 bit */
    unsigned long long sum = 0; /* total sum of the composition parts */
    unsigned parts[64];        /* differences between consecutive positions */
    int parts_cnt = 0;

    uint64_t mask = k;
    int bit_pos = 1;            /* positions are 1‑based, LSB = 1 */
    while (mask) {
        if (mask & 1ULL) {
            uint64_t pos = bit_pos;           /* current 1‑bit position */
            unsigned diff = (unsigned)(pos - prev);
            parts[parts_cnt++] = diff;
            sum += diff;
            prev = pos;
        }
        mask >>= 1;
        bit_pos++;
    }

    /* If the total sum is odd, it cannot be partitioned into two equal sums. */
    if (sum & 1ULL) return false;

    unsigned target = (unsigned)(sum >> 1);   /* half of the total sum */

    /* Subset‑sum DP using a 128‑bit bitset (target will never exceed 127). */
    unsigned __int128 dp = 1;                  /* dp[0] = 1 initially */
    for (int i = 0; i < parts_cnt; ++i) {
        unsigned p = parts[i];
        if (p <= target) {
            dp |= (dp << p);
        }
    }
    return ((dp >> target) & 1ULL) != 0;
}

seq_t a(idx_t n)
{
    /* Branch for the first term (k = 0). */
    if (n == 1) return (seq_t)0;

    idx_t count = 0;
    uint64_t k = 0;

    while (1) {
        if (is_biquanimous(k)) {
            ++count;
            if (count == n) return (seq_t)k;
        }
        ++k;
    }
}
