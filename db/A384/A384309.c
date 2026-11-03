/*
 * A384309  a(1) = 1. Thereafter a(n) is the cardinality of the set of terms
 *          whose leading decimal digit is the same as that of a(n-1).
 * https://oeis.org/A384309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(1)=1; for n>1, a(n)=number of previous terms whose leading
 *            decimal digit equals the leading digit of a(n-1)
 * Range:     u8 n <= 614, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9384 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static inline int leading_digit_seq_t(seq_t x)
{
    const seq_t ten = (seq_t)10;
    while (x >= ten) {
        x /= ten;
    }
    return (int)x;
}

/* Static cache for computed values and leading‑digit counts. */
static seq_t *seq_cache      = NULL;  /* array of a(i) */
static size_t seq_len        = 0;     /* number of values cached */
static size_t seq_cap        = 0;     /* allocated capacity */
static idx_t   leading_cnt[10] = {0,0,0,0,0,0,0,0,0,0};

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* out of bounds, not needed for OEIS offset 1 */

    /* Initialise cache on first call. */
    if (seq_cache == NULL) {
        seq_cap = 32;
        seq_cache = mem_new(seq_t, seq_cap);
        if (!seq_cache) return 0;              /* allocation failure */
        seq_cache[0] = (seq_t)1;                /* a(1) = 1 */
        seq_len = 1;
        leading_cnt[1] = 1;                     /* one term has leading digit 1 */
    }

    /* Compute terms up to n if needed. */
    while (seq_len < n) {
        seq_t last = seq_cache[seq_len - 1];
        int ld_last = leading_digit_seq_t(last);
        idx_t val_count = leading_cnt[ld_last];
        seq_t next = (seq_t)val_count;          /* conversion may overflow if seq_t is small */

        /* Grow cache if necessary. */
        if (seq_len == seq_cap) {
            seq_cap <<= 1;
            seq_cache = mem_resize(seq_t, seq_cache, seq_cap);
            if (!seq_cache) return 0;          /* allocation failure */
        }

        seq_cache[seq_len] = next;
        seq_len++;

        int ld_next = leading_digit_seq_t(next);
        leading_cnt[ld_next]++;                 /* new term observed */
    }

    return seq_cache[n - 1];                      /* a(1) stored at index 0 */
}
