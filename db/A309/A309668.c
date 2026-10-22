/*
 * A309668  a(n) is the least positive number of the form floor(2^k/n) for
 *          some k >= 0 not yet in the sequence.
 * https://oeis.org/A309668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = the least positive integer of the form floor(2^k / n)
 *            that has not appeared earlier for any index < n
 * Range:     u8 n <= 11, u16 n <= 67, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a cache of the sequence values and a bitmap
   marking which integers have already been used.  For each new index the
   algorithm scans k = 0,1,2,…, computing floor(2^k / n) until it finds
   an unused value, which is appended to the cache.  All operations
   involve integer arithmetic only, so the code works for any seq_t
   implementation supplied by the build.  The allocated arrays grow
   geometrically as needed, ensuring that repeated calls in any order
   return the correct term. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pow2.h"     /* pow2(idx_t) returns 2^exp as seq_t */

/* ---------- static cache ----------
 * seq   : array of computed terms
 * seq_len : number of terms already computed
 * seq_cap : current allocation size of seq
 *
 * used  : bitmap of which positive integers have been seen
 * used_cap : current allocation size of used
 */
static seq_t *seq = NULL;
static size_t seq_cap = 0;
static idx_t seq_len = 0;

static unsigned char *used = NULL;
static size_t used_cap = 0;

/* Ensure that 'used' can address index v.  It grows exponentially. */
static void ensure_used(idx_t v)
{
    if (used == NULL) {
        used_cap = 16;
        used = (unsigned char *)calloc(used_cap, 1);
        if (!used) abort();
    }
    while ((idx_t)used_cap <= v) {
        size_t new_cap = used_cap << 1;          /* double the capacity */
        unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
        if (!tmp) abort();
        memset(tmp + used_cap, 0, new_cap - used_cap);
        used = tmp;
        used_cap = new_cap;
    }
}

/* Ensure that seq has room for at least one more element. */
static void ensure_seq_cap(void)
{
    if (seq_len >= (idx_t)seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 16;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq = tmp;
        seq_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined but safe guard */

    /* Compute terms up to index n if they are not already cached. */
    for (idx_t i = seq_len + 1; i <= n; ++i) {
        idx_t k = 0;
        while (1) {
            /* floor(2^k / i) */
            seq_t val = pow2(k) / (seq_t)i;
            if (val == (seq_t)0) {          /* skip zero results */
                ++k;
                continue;
            }

            idx_t vidx = (idx_t)val;        /* index into the bitmap */
            ensure_used(vidx);
            if (!used[vidx]) {              /* first time we see val */
                used[vidx] = 1;
                ensure_seq_cap();
                seq[seq_len++] = val;
                break;
            }
            ++k;                             /* try next k */
        }
    }
    return seq[n - 1];
}
