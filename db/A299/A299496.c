/*
 * A299496  Solution a( ) of the complementary equation a(n) = b(n-1) + b(n-2)
 *          + b(n-3), where a(0) = 3, a(1) = 4, a(2) = 5; see Comments.
 * https://oeis.org/A299496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(n-1) + b(n-2) + b(n-3), with a(0)=3, a(1)=4, a(2)=5;
 *            b(0) is the least positive integer not among a(0),a(1),a(2); for
 *            n>1, b(n) is the least positive integer not among a(0)…a(n) and
 *            b(0)…b(n‑1)
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* --- static state ---------------------------------------------------- */

static seq_t *a_cache   = NULL;   /* a[0..] */
static size_t a_cap      = 0;
static idx_t  a_len      = 0;

static seq_t *b_cache   = NULL;   /* b[0..] */
static size_t b_cap      = 0;
static idx_t  b_len      = 0;

static bool *used       = NULL;   /* used[v] == true if v is already used */
static size_t used_cap   = 0;
static idx_t  next_candidate = 1; /* smallest integer not yet inspected */

/* --- memory helpers --------------------------------------------------- */

static void ensure_a_cap(idx_t needed)
{
    if ((idx_t)a_cap > needed) return;
    size_t new_cap = a_cap ? a_cap << 1 : 16;
    if (new_cap < (size_t)(needed + 1)) new_cap = needed + 1;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cap = new_cap;
}

static void ensure_b_cap(idx_t needed)
{
    if ((idx_t)b_cap > needed) return;
    size_t new_cap = b_cap ? b_cap << 1 : 16;
    if (new_cap < (size_t)(needed + 1)) new_cap = needed + 1;
    b_cache = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
    b_cap = new_cap;
}

static void ensure_used_cap(idx_t val)
{
    if ((idx_t)used_cap > val) return;
    size_t new_cap = used_cap ? used_cap << 1 : 16;
    while (new_cap <= (size_t)val) new_cap <<= 1;
    bool *new_used = (bool *)realloc(used, new_cap * sizeof(bool));
    /* initialise newly allocated entries to false */
    size_t added = new_cap - used_cap;
    memset(new_used + used_cap, 0, added * sizeof(bool));
    used = new_used;
    used_cap = new_cap;
}

/* --- sequence initialization ---------------------------------------- */

static void init_sequence(idx_t upto)
{
    /* initialise the state only once */
    if (a_len == 0) {
        /* ensure space for at least upto+1 terms */
        ensure_a_cap(upto + 1);
        ensure_b_cap(upto + 1);

        a_cache[0] = (seq_t)3;
        a_cache[1] = (seq_t)4;
        a_cache[2] = (seq_t)5;
        a_len = 3;

        /* a(1) and a(2) are already in the used set */
        ensure_used_cap(5);
        used[(size_t)4] = true;
        used[(size_t)5] = true;

        /* compute b(0) */
        seq_t cand = 1;
        while (cand == (seq_t)a_cache[0] ||
               cand == (seq_t)a_cache[1] ||
               cand == (seq_t)a_cache[2]) {
            ++cand;
        }
        b_cache[0] = cand;
        ensure_used_cap((idx_t)cand);
        used[(size_t)cand] = true;
        next_candidate = (idx_t)(cand + 1);
        b_len = 1;

        /* compute b(1) and b(2) */
        for (idx_t i = 1; i <= 2; ++i) {
            seq_t c = (seq_t)next_candidate;
            while (1) {
                ensure_used_cap((idx_t)c);
                if (!used[(size_t)c]) break;
                ++c;
            }
            b_cache[i] = c;
            used[(size_t)c] = true;
            next_candidate = (idx_t)(c + 1);
        }
        b_len = 3;
    }

    /* extend the computation up to `upto` */
    for (idx_t n = a_len; n <= upto; ++n) {
        ensure_a_cap(n);
        ensure_b_cap(n);
        /* a(n) = b(n-1) + b(n-2) + b(n-3) */
        seq_t val = b_cache[n - 1] + b_cache[n - 2] + b_cache[n - 3];
        a_cache[n] = val;

        /* mark a(n) as used for future b's */
        ensure_used_cap((idx_t)val);
        used[(size_t)val] = true;

        /* compute b(n) */
        seq_t cand = (seq_t)next_candidate;
        while (1) {
            ensure_used_cap((idx_t)cand);
            if (!used[(size_t)cand]) break;
            ++cand;
        }
        b_cache[n] = cand;
        used[(size_t)cand] = true;
        next_candidate = (idx_t)(cand + 1);

        ++a_len;
        ++b_len;
    }
}

/* --- public interface ---------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < a_len)   /* already computed */
        return a_cache[n];

    init_sequence(n);
    return a_cache[n];
}
