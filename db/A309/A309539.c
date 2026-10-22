/*
 * A309539  Positive numbers that contain both odd and even digits, with no
 *          digit repeated.
 * https://oeis.org/A309539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   not stated
 * Range:     u8 n <= 135, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) is the nth smallest positive integer whose decimal digits are all
 * distinct and which contains at least one odd digit and at least one even
 * digit.  The sequence is finite (max term 9876543210).  This implementation
 * searches sequentially for the required numbers, caching the results so
 * that repetitive calls are fast.
 */

#include <stdlib.h>
#include <stdbool.h>

/* helper that tests whether x satisfies the sequence condition */
static inline bool valid_candidate(seq_t x)
{
    bool seen[10] = {0};
    bool has_odd = false;
    bool has_even = false;
    seq_t tmp = x;

    if (x == 0) return false;   /* positive integers only */

    while (tmp > 0) {
        unsigned d = (unsigned)(tmp % 10);
        if (seen[d]) return false;      /* repeated digit */
        seen[d] = true;
        if (d & 1) has_odd = true;       /* odd digit */
        else has_even = true;            /* even digit */
        tmp /= 10;
    }
    return has_odd && has_even;
}

seq_t a(idx_t n)
{
    /* static cache for already computed terms */
    static seq_t *cache = NULL;   /* array of seq_t values */
    static size_t cache_sz = 0;   /* number of terms stored */
    static size_t cache_cap = 0;  /* allocated capacity */
    static seq_t next_scan = 1;   /* next integer to examine */

    if (n == 0) return 0; /* undefined, but avoid underflow */

    /* Ensure we have computed up to term n */
    while (cache_sz < (size_t)n) {
        /* Find next valid candidate */
        seq_t cand = next_scan;
        while (!valid_candidate(cand)) {
            cand++;
        }

        /* Ensure capacity */
        if (cache_sz == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
            seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_ptr) abort(); /* memory error */
            cache = new_ptr;
            cache_cap = new_cap;
        }

        cache[cache_sz++] = cand;
        next_scan = cand + 1;
    }

    return cache[n - 1];
}
