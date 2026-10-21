/*
 * A296869  Numbers whose base-5 digits d(m), d(m-1), ..., d(0) have #(pits) <
 *          #(peaks); see Comments.
 * https://oeis.org/A296869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   this function searches for the n‑th integer whose * base‑5
 *            representation has more peaks than pits * (a pit is d(i‑1) >
 *            d(i) < d(i+1); a peak is d(i‑1) < d(i) > d(i+1))
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* External integer types supplied by the build system.  No redefinition. */
extern seq_t a(idx_t n);

/* Static cache of computed terms */
static seq_t *cache      = NULL;   /* array of cached a(k) values */
static idx_t  cache_len  = 0;      /* number of terms currently cached */
static idx_t  next_candidate = 1;  /* next integer to test */

/*
 * Return the n-th element of the sequence (n >= 1).
 *
 * The function lazily generates terms by brute‑force inspection of
 * successive integers in increasing order.  A static cache retains all
 * previously discovered terms so that calls are O(1) after the needed
 * terms have been computed.
 */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n = 0; return 0 for safety. */
        return (seq_t)0;
    }

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        idx_t candidate = next_candidate;

        /* Convert candidate to base‑5 digits, least‑significant first. */
        uint8_t digits[64];
        size_t len = 0;
        idx_t t = candidate;
        if (t == 0) {
            digits[len++] = 0;
        } else {
            while (t > 0) {
                digits[len++] = (uint8_t)(t % 5);
                t /= 5;
            }
        }

        /* Count pits and peaks. */
        size_t pits  = 0;
        size_t peaks = 0;
        if (len >= 3) {
            for (size_t i = 1; i + 1 < len; ++i) {
                uint8_t d_prev = digits[i - 1];
                uint8_t d_cur  = digits[i];
                uint8_t d_next = digits[i + 1];
                if (d_prev > d_cur && d_cur < d_next) {
                    ++pits;
                } else if (d_prev < d_cur && d_cur > d_next) {
                    ++peaks;
                }
            }
        }

        /* Store the candidate if it satisfies the condition. */
        if (peaks > pits) {
            seq_t *tmp = (seq_t *)realloc(cache,
                                          (cache_len + 1) * sizeof(seq_t));
            if (tmp == NULL) {
                abort(); /* Allocation failure */
            }
            cache = tmp;
            cache[cache_len] = (seq_t)candidate;
            ++cache_len;
        }

        /* Move to the next integer to test. */
        ++next_candidate;
    }

    /* Return the requested term from the cache. */
    return cache[n - 1];
}
