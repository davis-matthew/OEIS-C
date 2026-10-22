/*
 * A348235  List of distinct squared distances from all points of Don
 *          Wilkinson's 123-circle packing to a fixed type (c) point.
 * https://oeis.org/A348235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate all squared distances from a fixed type‑c point to all
   circle centers using:
   - type a (radius 1): 16*k^2 + 36*j^2   (k odd)
   - type b (radius 2): 16*k^2 + 9*m^2    (k,m odd)
   - type c (radius 3): 64*i^2 + 36*j^2
   Then collect distinct values up to a safe bound and return the
   nth smallest.
*/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* The build supplies seq_t and idx_t */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined but safe */

    /* Estimate a safe upper bound for the nth value:
       For n up to 10000, values grow roughly 17 per term.
       We use a generous multiplier to avoid retrying. */
    uint64_t L = (uint64_t)n * 20 + 1000ULL;

    /* Allocate a bitmap of seen values. */
    uint8_t *seen = (uint8_t *)calloc(L + 1, 1);
    if (!seen) return 0; /* allocation failure – unlikely */

    /* ----- type c: centers (8i+4, 6j) ----- */
    for (uint64_t i = 0; 64ULL * i * i <= L; ++i) {
        uint64_t part = 64ULL * i * i;
        for (uint64_t j = 0; part + 36ULL * j * j <= L; ++j) {
            seen[part + 36ULL * j * j] = 1;
        }
    }
    /* ----- type a: centers (8i, 6j) ----- */
    for (uint64_t k = 1; 16ULL * k * k <= L; k += 2U) { /* k odd */
        uint64_t part = 16ULL * k * k;
        for (uint64_t j = 0; part + 36ULL * j * j <= L; ++j) {
            seen[part + 36ULL * j * j] = 1;
        }
    }
    /* ----- type b: centers (8i, 6j+3) ----- */
    for (uint64_t k = 1; 16ULL * k * k <= L; k += 2U) { /* k odd */
        uint64_t part = 16ULL * k * k;
        for (uint64_t m = 1; part + 9ULL * m * m <= L; m += 2U) { /* m odd */
            seen[part + 9ULL * m * m] = 1;
        }
    }

    /* Scan to find the nth distinct value. */
    seq_t result = 0;
    uint64_t count = 0;
    for (uint64_t val = 0; val <= L; ++val) {
        if (seen[val]) {
            ++count;
            if (count == (uint64_t)n) {
                result = (seq_t)val;
                break;
            }
        }
    }

    free(seen);
    return result;
}
