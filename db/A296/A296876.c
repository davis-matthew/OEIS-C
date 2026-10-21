/*
 * A296876  Numbers whose base-8 digits d(m), d(m-1), ..., d(0) have #(pits) =
 *          #(peaks); see Comments.
 * https://oeis.org/A296876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: generate A296876 by scanning integers and counting pits and
   peaks in their base‑8 representation; keep a cache so any n can be
   answered in O(n) time. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Count whether an integer has equal numbers of pits and peaks in base‑8. */
static bool is_good(idx_t n)
{
    /* digits in little‑endian order: d[0] = least significant digit */
    uint8_t d[64];
    size_t len = 0;
    do {
        d[len++] = (uint8_t)(n & 7);   /* n % 8 */
        n >>= 3;                       /* n /= 8 */
    } while (n);

    /* Less than 3 digits => no internal positions => equal counts (both 0). */
    if (len <= 2) return true;

    size_t pits = 0, peaks = 0;
    for (size_t i = 1; i + 1 < len; ++i) {
        uint8_t left  = d[i - 1];
        uint8_t mid   = d[i];
        uint8_t right = d[i + 1];
        if (left > mid && mid < right) {
            ++pits;
        } else if (left < mid && mid > right) {
            ++peaks;
        }
    }
    return pits == peaks;
}

seq_t a(idx_t n)
{
    static seq_t *terms     = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static idx_t  last_tested = 0;

    if (n == 0) return 0;   /* safety for out‑of‑range indices */

    /* Extend the cache until it holds at least n elements. */
    if (terms_len < (size_t)n) {
        size_t need = (size_t)n + 32;
        if (terms_cap < need) {
            size_t new_cap = terms_cap ? terms_cap * 2 : 64;
            while (new_cap < need) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            terms = tmp;
            terms_cap = new_cap;
        }

        /* Scan successive integers starting just after the last tested one. */
        seq_t cand = (seq_t)(last_tested + 1);
        while (terms_len < (size_t)n) {
            if (is_good((idx_t)cand)) {
                terms[terms_len++] = cand;
            }
            ++cand;
        }
        last_tested = (idx_t)(cand - 1);
    }

    return terms[n - 1];
}
