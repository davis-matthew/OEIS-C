/*
 * A247811  Numbers in decimal representation with distinct digits, such that
 *          in Russian their digits are in alphabetic order.
 * https://oeis.org/A247811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, word, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 189, u32 n <= 959, u64 n <= 960,
 *            u128 n <= 960, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by enumerating all non‑empty subsets of {0,…,9} and
   constructing each number by writing the chosen digits in the
   order 8,2,9,0,1,5,7,3,4,6 (the alphabetical order of their
   transliterated Russian names).  Subsets that would yield a leading
   zero (except the single digit 0) are discarded.  The resulting
   numbers are sorted numerically to obtain the sequence. */

#include <stdlib.h>

/* Order of digits according to Cyrillic transliteration of
   their Russian names: 8,2,9,0,1,5,7,3,4,6. */
static const int ORDER[10] = {8, 2, 9, 0, 1, 5, 7, 3, 4, 6};

static const idx_t ZERO_BIT   = ((idx_t)1 << 0);
static const idx_t LEADING_MASK = ((idx_t)1 << 8) | ((idx_t)1 << 2) |
                                  ((idx_t)1 << 9);  /* digits 8,2,9 */

/* --- comparison function for qsort ---------------------------------- */
static int compare_seq(const void *p, const void *q)
{
    const seq_t *x = (const seq_t *)p;
    const seq_t *y = (const seq_t *)q;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* ------------------------------------------------------------------- */

static seq_t CACHE[1024];
static size_t CACHE_LEN = 0;
static int BUILT = 0;

seq_t a(idx_t n)
{
    /* Build the sequence once.  A maximum of 1023 non‑empty subsets
       exist; after filtering we store at most that many values.       */
    if (!BUILT) {
        CACHE_LEN = 0;

        const idx_t LIM = ((idx_t)1 << 10);          /* 2^10  = 1024   */
        for (idx_t mask = 1; mask < LIM; ++mask) {
            /* Skip leading‐zero numbers that are not the singleton 0 */
            if ((mask & ZERO_BIT) && (mask != ZERO_BIT) &&
                (mask & LEADING_MASK) == 0)
                continue;

            seq_t val = 0;
            for (size_t i = 0; i < 10; ++i) {
                int d = ORDER[i];
                if (mask & ((idx_t)1 << d)) {
                    val = val * ((seq_t)10) + (seq_t)d;
                }
            }
            CACHE[CACHE_LEN++] = val;
        }

        qsort(CACHE, CACHE_LEN, sizeof(seq_t), compare_seq);
        BUILT = 1;
    }

    if (n == 0) return 0;                         /* undefined below offset */
    if (n <= CACHE_LEN) {
        /* n is guaranteed to be <= CACHE_LEN, so n-1 fits in size_t  */
        return CACHE[(size_t)(n - 1)];
    }
    return 0;                                     /* out of bounds */
}
