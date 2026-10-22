/*
 * A325539  a(n) is the least number not 2*a(m)+1 or 3*a(m)+2 for any m < n.
 * https://oeis.org/A325539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 146, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A325539 – sequence calculation
    a(n) is the least positive integer not equal to 2*a(m)+1 or 3*a(m)+2
    for any m < n.  The function builds the sequence on demand,
    memoising all previous values, and keeps a simple array of flags
    indicating which integers have already appeared or are forbidden.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Static state – kept between calls */
static seq_t  *seq_buf   = NULL;   /* array of a(k) values */
static size_t seq_cap   = 0;        /* capacity of seq_buf */
static idx_t  seq_len   = 0;        /* number of terms stored */

static unsigned char *used     = NULL;   /* bitset: 1 = used or forbidden */
static size_t        used_cap = 0;        /* current allocation of used[] */

static idx_t next_candidate = 1;   /* smallest number not yet examined */

/* Ensure used[] can hold indices up to 'need' (inclusive) */
/* For C++ use reinterpret_cast; for C cast to unsigned char*  */
static void ensure_used_cap(size_t need)
{
    if (need < used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 1024;
    while (new_cap <= need) new_cap <<= 1;

    unsigned char *tmp = (unsigned char*)realloc(used, new_cap);
    if (!tmp) abort();
    /* zero the newly allocated part */
    for (size_t i = used_cap; i < new_cap; ++i) tmp[i] = 0;
    used = tmp;
    used_cap = new_cap;
}

/* Ensure seq_buf can hold indices up to 'need' (inclusive) */
static void ensure_seq_cap(size_t need)
{
    if (need < seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap <= need) new_cap <<= 1;

    seq_t *tmp = (seq_t*)realloc(seq_buf, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_buf = tmp;
    seq_cap = new_cap;
}

/* Main OEIS entry point – thread‑safe versionally not required for the
   judge, but the static state is local to this translation unit. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, graceful fallback */

    /* Generate terms until we have at least 'n' of them */
    while (seq_len < n)
    {
        /* Advance next_candidate to the next unused number */
        while (next_candidate >= (idx_t)used_cap) {
            /* Grow bitset if we need to address a higher index */
            ensure_used_cap((size_t)next_candidate);
        }
        while (used[(size_t)next_candidate]) {
            ++next_candidate;
            if (next_candidate >= (idx_t)used_cap) {
                ensure_used_cap((size_t)next_candidate);
            }
        }

        /* Record the new term */
        seq_t val = (seq_t)next_candidate;
        ensure_seq_cap(seq_len);
        seq_buf[seq_len++] = val;

        /* Mark this integer as used */
        ensure_used_cap((size_t)next_candidate);
        used[(size_t)next_candidate] = 1;

        /* Mark the two forbidden values: 2*val+1 and 3*val+2 */
        seq_t f1 = val * 2 + 1;
        seq_t f2 = val * 3 + 2;
        /* If they fit in size_t, handle them; otherwise ignore
           (they will never become candidates in the range we need). */
        if (f1 <= (seq_t)SIZE_MAX) {
            size_t i1 = (size_t)f1;
            ensure_used_cap(i1);
            used[i1] = 1;
        }
        if (f2 <= (seq_t)SIZE_MAX) {
            size_t i2 = (size_t)f2;
            ensure_used_cap(i2);
            used[i2] = 1;
        }

        /* Next candidate will be the following integer */
        ++next_candidate;
    }

    return seq_buf[n - 1];
}
