/*
 * A355632  Irregular triangle T(n, k), n > 0, k = 1..A121041(n), read by
 *          rows; the n-th row contains in ascending order the divisors of n
 *          whose decimal expansions appear as substrings in the decimal
 *          expansion of n.
 * https://oeis.org/A355632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   a(n) is the n‑th term of the flattened triangle * T(m,k) =
 *            divisors of m that appear as decimal substrings of m. * The
 *            triangle is read row by row: a(1) = 1, a(2) = 2, a(3) = 3, …
 * Range:     u8 n <= 560, u16 n <= 8462, u32 n <= 8462, u64 n <= 8462,
 *            u128 n <= 8462, big any n
 * Helpers:   uint_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../utils/uint_to_str.h"

static int cmp_uint64(const void *p1, const void *p2)
{
    uint64_t a = *(const uint64_t *)p1;
    uint64_t b = *(const uint64_t *)p2;
    return (a > b) - (a < b);
}

/* Integer square root: floor(sqrt(x)) */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = x;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        if (mid > 0 && mid > x / mid) {
            hi = mid - 1;
        } else {
            if ((mid + 1) > 0 && (mid + 1) > x / (mid + 1))
                return mid;
            lo = mid + 1;
        }
    }
    return hi; /* should not be reached */
}

/* Check whether the decimal representation of sub occurs as a substring in full. */
static int contains_substring(uint64_t full, uint64_t sub)
{
    char fbuf[32], sbuf[32];
    uint_to_str(full, fbuf);
    uint_to_str(sub, sbuf);
    return strstr(fbuf, sbuf) != NULL;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined index, return 0 safely */
        return (seq_t)0;

    idx_t target = n;   /* 1‑based target position in the flattened triangle */
    idx_t row    = 1;   /* current row number */
    idx_t cum    = 0;   /* cumulative count of all entries processed so far */

    /* A static buffer large enough for all divisors of any realistic row. */
    static uint64_t divs[8192];
    size_t sz;

    while (1) {
        /* --- gather all divisors of the current row --- */
        sz = 0;
        uint64_t lim = isqrt_u64(row);
        for (uint64_t d = 1; d <= lim; ++d) {
            if (row % d == 0) {
                divs[sz++] = d;
                if (d != row / d)
                    divs[sz++] = row / d;
            }
        }

        /* sort divisors in ascending order */
        qsort(divs, sz, sizeof(uint64_t), cmp_uint64);

        /* Count how many of these divisors appear as substrings. */
        idx_t matchcnt = 0;
        for (size_t i = 0; i < sz; ++i)
            if (contains_substring(row, divs[i]))
                ++matchcnt;

        /* If the desired element lies in this row, pick it. */
        if (cum + matchcnt >= target) {
            idx_t need = target - cum;   /* 1‑based index within this row */
            idx_t matched = 0;
            for (size_t i = 0; i < sz; ++i) {
                if (contains_substring(row, divs[i])) {
                    ++matched;
                    if (matched == need)
                        return (seq_t)divs[i];
                }
            }
            /* Should never reach here */
            return (seq_t)0;
        }

        /* Move to the next row and continue. */
        cum += matchcnt;
        ++row;
    }
}
