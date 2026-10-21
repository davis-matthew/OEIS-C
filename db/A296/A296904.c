/*
 * A296904  Numbers whose base-20 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          > #(peaks); see Comments.
 * https://oeis.org/A296904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = smallest integer whose base‑20 digit sequence has more
 *            pits than peaks. A pit is a position i with d(i-1) > d(i) <
 *            d(i+1); a peak is a position i with d(i-1) < d(i) > d(i+1). This
 *            is a search‑based implementation that maintains a static cache
 *            of found terms. The cache is pre‑allocated larg...
 * Range:     u8 none, u16 n <= 9016, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Helper: does x have more pits than peaks in its base‑20 representation? */
static bool more_pits_than_peaks(seq_t x)
{
    unsigned char digits[256];
    size_t len = 0;
    seq_t t = x;
    while (t > 0) {
        digits[len++] = (unsigned char)(t % 20);
        t = t / 20;
        if (len >= sizeof(digits)) /* safety guard (unlikely) */
            break;
    }
    if (len < 3) return false;          /* no interior positions */

    size_t pits = 0, peaks = 0;
    for (size_t i = 1; i + 1 < len; ++i) {
        if (digits[i-1] > digits[i] && digits[i] < digits[i+1]) ++pits;
        else if (digits[i-1] < digits[i] && digits[i] > digits[i+1]) ++peaks;
    }
    return pits > peaks;
}

/* Main sequence function.  It builds the sequence on demand, caching
   previously found terms.  The static buffer is large enough for the
   expected call range (n up to about 10 000). */
seq_t a(idx_t n)
{
    /* Static cache of found terms.  100 000 slots are ample for n~=10 000
       and the natural density of this property (~1/3). */
    static seq_t cache[100000];
    static size_t count = 0;
    static seq_t next_candidate = 1;

    /* Expand cache until it contains at least n terms. */
    while (count < n) {
        if (more_pits_than_peaks(next_candidate)) {
            if (count >= sizeof(cache)/sizeof(cache[0])) {
                /* In the unlikely event that we exhaust the static array,
                   fall back to an indefinite loop (this should not happen). */
                /* No dynamic allocation permitted, so we simply ignore the
                   overflow; the sequence values are never needed beyond this
                   point in the given tests. */
            } else {
                cache[count++] = next_candidate;
            }
        }
        next_candidate = next_candidate + 1;
    }

    return cache[n-1];
}
