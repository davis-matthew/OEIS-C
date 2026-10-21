/*
 * A261213  Odd numbers n such that n^2 = m + (m+1), where both m and m+1 have
 *          no repeated digits.
 * https://oeis.org/A261213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 35, u32 n <= 146, u64 n <= 146,
 *            u128 n <= 146, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: odd n such that (n^2 - 1)/2 and (n^2 + 1)/2 have no repeated digits.
 * Computed by searching odd n and testing the two consecutive numbers for
 * distinct decimal digits.
 */
#include <stdbool.h>
#include <stddef.h>

/* Static storage for precomputed terms. */
static seq_t terms[200];
static size_t terms_count = 0;
static bool terms_initialized = false;

/* Helper: return true if x contains no repeated decimal digits. */
static bool has_unique_digits(seq_t x)
{
    unsigned char used[10] = {0};
    while (x > 0) {
        int d = (int)(x % 10);
        if (used[d]) return false;
        used[d] = 1;
        x /= 10;
    }
    return true;
}

/* Initialize the terms array by brute–force search up to a reasonable bound. */
static void init_terms(void)
{
    if (terms_initialized) return;
    idx_t cand = 1;
    for (; cand <= 1000000; cand += 2) {   /* search enough to capture all terms */
        seq_t ncand = (seq_t)cand;
        seq_t sq    = ncand * ncand;
        seq_t m     = (sq - 1) / 2;          /* (n^2 - 1) / 2 */
        if (has_unique_digits(m) && has_unique_digits(m + 1)) {
            if (terms_count < sizeof(terms)/sizeof(terms[0])) {
                terms[terms_count++] = ncand;
            }
        }
        /* The known last term is 40797, so we can stop early when we have
           exceeded this value and found all terms. */
        if (cand >= 50000 && terms_count >= 150) break;
    }
    terms_initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    init_terms();
    if ((size_t)(n - 1) < terms_count) {
        return terms[n - 1];
    }
    /* n beyond known terms – return 0 for safety. */
    return 0;
}
