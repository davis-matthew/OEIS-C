/*
 * A336307  Numbers that are neither Colombian nor Brazilian.
 * https://oeis.org/A336307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   digit_sum, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers that are neither Colombian nor Brazilian. */
/* Computed by exhaustive search with memoisation. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/digit_sum.h"
#include "../utils/floor_sqrt.h"

/* Helper: does n = m + digit_sum(m) for some 1 ≤ m < n ? */
static bool is_colombian(seq_t n)
{
    /* Count decimal digits of n */
    unsigned int d = 0;
    seq_t tmp = n;
    while (tmp > 0) {
        ++d;
        tmp /= 10;
    }
    if (d == 0) d = 1;   /* n == 0 is not considered, but safety */

    /* m must be in [n-9*d, n-1] and at least 1 */
    seq_t nine_d = (seq_t)d * 9;
    seq_t low = (n > nine_d) ? (n - nine_d) : 1;

    for (seq_t m = low; m < n; ++m) {
        if (n == m + digit_sum(m)) {
            return true;
        }
    }
    return false;
}

/* Helper: is n a Brazilian number?  */
static bool is_brazilian(seq_t n)
{
    /* Base range is 2 .. n-2 */
    if (n < 4) return false;  /* no base in the range */

    idx_t n_idx = (idx_t)n;
    idx_t max_b = n_idx - 2;
    if (max_b < 2) return false;

    /* Compute floor(sqrt(n)) */
    idx_t sqrt_n = (idx_t)floor_sqrt(n);
    if (sqrt_n > max_b) sqrt_n = max_b;

    /* Bases with length ≥ 3 (b ≤ sqrt(n)) */
    for (idx_t b = 2; b <= sqrt_n; ++b) {
        seq_t tmp = n;
        seq_t first_digit = 0;
        bool same = true;
        unsigned int len = 0;
        while (tmp > 0) {
            seq_t digit = tmp % b;
            if (len == 0)
                first_digit = digit;
            else if (digit != first_digit) {
                same = false;
                break;
            }
            tmp /= b;
            ++len;
        }
        if (same && len >= 2) return true;
    }

    /* Bases where representation length is exactly 2 (b > sqrt(n)) */
    for (idx_t b = sqrt_n + 1; b <= max_b; ++b) {
        seq_t denom = (seq_t)b + 1;
        if (n % denom != 0) continue;
        seq_t d = n / denom;
        if (d >= 1 && d < (seq_t)b) return true;
    }

    return false;
}

/* Sequence cache: seq[0] corresponds to a(1) */
static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* Public routine required by the harness. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Grow cache until we have at least n terms. */
    while (seq_len < n) {
        seq_t cand = (seq_len == 0) ? 2 : seq[seq_len - 1] + 1;

        /* Search forward for the next valid term. */
        while (true) {
            if (is_colombian(cand) && !is_brazilian(cand)) {
                /* Append to cache. */
                if (seq_len == seq_cap) {
                    size_t new_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
                    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                    seq_cap = new_cap;
                }
                seq[seq_len++] = cand;
                break;
            }
            ++cand;
        }
    }
    return seq[n - 1];
}
