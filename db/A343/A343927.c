/*
 * A343927  a(1) = 1, a(2) = 2; for n>2, a(n) is the smallest positive integer
 *          not yet in the sequence which shares a digit with a(n-1) but not
 *          with a(n-2), and where a(n) contains at least one digit not in
 *          a(n-1).
 * https://oeis.org/A343927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A343927
    a(1) = 1, a(2) = 2; for n>2, a(n) is the smallest unused positive integer
    that shares at least one digit with a(n‑1), shares no digit with a(n‑2),
    and contains at least one digit not present in a(n‑1).
    This implementation generates terms on demand, caching them and keeping
    a bitmap of used numbers.  The algorithm is O(n·M) where M is the
    maximum value that occurs, which is well below the limits for n≈10000.   */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* Helper: compute bitmask of decimal digits of x */
static inline uint16_t digit_mask(seq_t x)
{
    uint16_t m = 0;
    while (x != 0) {
        int d = (int)(x % 10);
        m |= 1u << d;
        x /= 10;
    }
    return m;
}

/* Global state: sequence cache and used-number bitmap */
static seq_t *seq_cache  = NULL;
static size_t seq_cap    = 0;
static size_t seq_len    = 0;   /* number of elements stored */

static unsigned char *used_bits = NULL;
static size_t used_cap   = 0;   /* number of bytes allocated for used bits */

static bool initialized = false;

/* Ensure seq_cache has capacity for 'need' elements */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Ensure used_bits has capacity for numbers up to 'num' (inclusive) */
static void ensure_used_capacity(size_t num)
{
    if (num < used_cap) return;
    size_t new_cap = used_cap ? used_cap << 1 : 128;
    while (new_cap <= num) new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(used_bits, new_cap);
    if (!tmp) return;                /* allocation failure: give up */
    if (new_cap > used_cap) {
        memset(tmp + used_cap, 0, new_cap - used_cap);
    }
    used_bits = tmp;
    used_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* initialize on first call */
    if (!initialized) {
        /* initial values */
        ensure_seq_capacity(2);
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)2;
        seq_len = 2;
        /* bitmap for used numbers */
        ensure_used_capacity(2);
        used_bits[0] = 0;                /* 0 is never used */
        used_bits[1] = 1;                /* used: 1 */
        used_bits[2] = 1;                /* used: 2 */
        initialized = true;
    }

    /* extend sequence up to index n (1‑based) */
    while (n > seq_len) {
        seq_t last   = seq_cache[seq_len - 1];          /* a(n-1) */
        seq_t prev   = seq_cache[seq_len - 2];          /* a(n-2) */
        uint16_t mask_last   = digit_mask(last);
        uint16_t mask_prev   = digit_mask(prev);

        /* search for the smallest candidate */
        size_t cand = 1;                      /* start from 1 each time */
        for (;;) {
            if (cand > used_cap) ensure_used_capacity(cand);
            if (!used_bits[cand]) {
                seq_t cand_t = (seq_t)cand;
                uint16_t m = digit_mask(cand_t);
                bool share_with_last   = (m & mask_last) != 0;
                bool share_with_prev   = (m & mask_prev) != 0;
                bool new_digit         = (m & ~mask_last) != 0;
                if (share_with_last && !share_with_prev && new_digit) {
                    /* candidate found */
                    used_bits[cand] = 1;
                    ensure_seq_capacity(seq_len + 1);
                    seq_cache[seq_len] = cand_t;
                    seq_len++;
                    break;
                }
            }
            cand++;
        }
    }

    return seq_cache[n - 1];   /* convert from 1‑based index to 0‑based array */
}
