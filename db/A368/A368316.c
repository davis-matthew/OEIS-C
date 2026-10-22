/*
 * A368316  Lexicographically earliest sequence of distinct nonnegative
 *          integers such that for any n >= 0, a(n) and Sum_{k = 0..n-1} a(k)
 *          can be added without carries in balanced ternary.
 * https://oeis.org/A368316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   to_balanced
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A368316: lexicographically earliest sequence of distinct nonnegative
   integers such that for any n ≥ 0, a(n) and Sum_{k=0..n-1} a(k)
   add without carries in balanced ternary.
   Implementation: iteratively build the sequence, caching already‑computed
   terms, keeping a running sum and its balanced‑ternary digits, and
   marking used numbers.  The next term is the smallest unused non‑negative
   integer whose binary‑ternary digit set does not overlap the
   corresponding digits of the current sum.  The code works for any
   seq_t type (built‑in integer or arbitrary‑precision big integer) and
   any idx_t type (uint64_t).  All allocations use C's realloc and are
   cast explicitly so that the program compiles as C++ too.  */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/to_balanced.h"

/* running sum of the sequence so far (in the natural base) */
static seq_t seq_sum = 0;

/* balanced‑ternary digits of seq_sum, least significant first.
   3‑digit values are stored as -1, 0 or 1.  Size 2048 comfortably covers
   the expected digit lengths for the required n. */
static signed char Sbuf[2048];
static size_t Slen = 0;

/* bitmap of numbers already used in the sequence */
static unsigned char *used = NULL;
static size_t used_cap = 0;

/* cached values of a(n) so we can answer out‑of‑order queries */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* make sure that `used` can index up to `idx`.  Grow the array if
   necessary.  The new part is zero‑initialized. */
static void ensure_used_cap(size_t idx)
{
    if (idx < used_cap)
        return;
    size_t new_cap = used_cap ? used_cap * 2 : 64;
    while (idx >= new_cap)
        new_cap *= 2;
    unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
    if (!tmp)
        exit(1);          /* allocation failure */
    memset(tmp + used_cap, 0, new_cap - used_cap);
    used = tmp;
    used_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* make sure that `cache` can hold at least `needed` elements.
   (Indices 0 .. needed-1).  The new part need not be initialized
   because we only write values before reading them. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (needed > new_cap)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* test whether `cand` is unused and adding it to the current sum
   would cause a carry in balanced ternary; that is, no digit position
   contains the same non‑zero digit in both numbers. */
static bool candidate_ok(idx_t cand)
{
    ensure_used_cap((size_t)cand);
    if (used[(size_t)cand])
        return false;

    /* balanced‑ternary digits of the candidate */
    signed char cand_buf[2048];
    size_t cand_len;
    to_balanced((seq_t)cand, cand_buf, &cand_len);

    size_t maxlen = Slen > cand_len ? Slen : cand_len;
    for (size_t i = 0; i < maxlen; ++i) {
        signed char sd = (i < Slen) ? Sbuf[i] : 0;
        signed char cd = (i < cand_len) ? cand_buf[i] : 0;
        if (sd != 0 && cd != 0 && sd == cd)
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* find the smallest non‑negative integer satisfying the constraints. */
static idx_t find_min_candidate(void)
{
    idx_t cand = 0;
    while (true) {
        if (candidate_ok(cand))
            return cand;
        cand++;
    }
}

/* ------------------------------------------------------------------ */
/* return the n‑th term of the sequence.  Values are cached so that
   calls in any order are handled efficiently. */
seq_t a(idx_t n)
{
    if (n < cache_len)
        return cache[n];

    ensure_cache_cap(n + 1);

    while (cache_len <= n) {
        idx_t cand = find_min_candidate();

        /* mark candidate as used */
        ensure_used_cap((size_t)cand);
        used[(size_t)cand] = 1;

        /* update running sum and its digits */
        seq_sum += (seq_t)cand;
        to_balanced(seq_sum, Sbuf, &Slen);

        /* store the new term */
        cache[cache_len++] = (seq_t)cand;
    }
    return cache[n];
}
