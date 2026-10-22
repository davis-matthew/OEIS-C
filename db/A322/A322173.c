/*
 * A322173  Base-6 deletable primes (written in base 10).
 * https://oeis.org/A322173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 3560, u32 n <= 3561, u64 n <= 3561,
 *            u128 n <= 3561, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A322173: Base-6 deletable primes (written in base 10).
   The function computes the sequence by an incremental search, using
   memoisation to avoid repeated recursive checks of the deletable
   property. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"   /* for bool is_prime_big(seq_t) */

typedef struct {
    seq_t p;
    int   val;   /* 1 = deletable, 0 = not deletable */
} CacheItem;

/* --------- Memoisation cache for the deletable property ---------- */
static CacheItem *cache = NULL;      /* sorted by p */
static size_t    cache_len = 0;
static size_t    cache_cap = 0;

static void cache_ensure(size_t need)
{
    if (need > cache_cap) {
        size_t newcap = cache_cap ? cache_cap * 2 : 8;
        while (newcap < need) newcap *= 2;
        cache = (CacheItem *)realloc(cache, newcap * sizeof(CacheItem));
        cache_cap = newcap;
    }
}

static int cache_lookup(seq_t p, int *found)
{
    size_t lo = 0, hi = cache_len;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (cache[mid].p < p) lo = mid + 1;
        else if (cache[mid].p > p) hi = mid;
        else {
            *found = cache[mid].val;
            return 1;
        }
    }
    return 0;
}

static void cache_insert(seq_t p, int val)
{
    cache_ensure(cache_len + 1);
    /* binary search for insertion point */
    size_t lo = 0, hi = cache_len;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (cache[mid].p < p) lo = mid + 1;
        else hi = mid;
    }
    size_t pos = lo;
    /* shift items to the right */
    for (size_t k = cache_len; k > pos; --k) cache[k] = cache[k - 1];
    cache[pos].p   = p;
    cache[pos].val = val;
    ++cache_len;
}

/* ---------- Recursive test of the deletable property ------------- */
static int is_deletable(seq_t p)
{
    int found;
    if (cache_lookup(p, &found)) return found;

    /* Extract base‑6 digits into an array (most significant first) */
    int    digits[64];
    size_t dlen = 0;
    seq_t  tmp = p;
    /* Special case: zero (not a prime) is never reached */
    while (tmp > 0) {
        digits[dlen++] = (int)(tmp % 6);
        tmp /= 6;
    }
    if (dlen == 0) { /* p == 0, not a prime */
        cache_insert(p, 0);
        return 0;
    }
    /* Reverse to get most significant first */
    for (size_t i = 0; i < dlen / 2; ++i) {
        int t = digits[i];
        digits[i] = digits[dlen - 1 - i];
        digits[dlen - 1 - i] = t;
    }

    /* If only one digit, deletion can give the empty string */
    if (dlen == 1) {
        cache_insert(p, 1);
        return 1;
    }

    /* Try all possible deletions */
    for (size_t del = 0; del < dlen; ++del) {
        bool leadingZero = false;
        bool firstSeen   = false;
        seq_t newp = 0;
        for (size_t j = 0; j < dlen; ++j) {
            if (j == del) continue;
            int d = digits[j];
            if (!firstSeen) {
                firstSeen = true;
                if (d == 0) leadingZero = true;
            }
            newp = newp * (seq_t)6 + d;
        }
        if (leadingZero) continue;          /* leading zeros not allowed */
        if (!is_prime_big(newp)) continue;  /* must stay prime */
        if (is_deletable(newp)) {           /* recursive step */
            cache_insert(p, 1);
            return 1;
        }
    }

    cache_insert(p, 0);
    return 0;
}

/* -------------------- Sequence a(n) ---------- */
seq_t a(idx_t n)
{
    /* Static storage for previously computed sequence values */
    static seq_t *terms    = NULL;
    static size_t terms_len = 0, terms_cap = 0;
    static uint64_t next_candidate = 2; /* the next number to test */

    if (n == 0) return 0; /* offset is 1; undefined for n==0 */

    while (terms_len < n) {
        uint64_t cand = next_candidate;
        /* Prepare next candidate: skip even numbers after 2 */
        if (cand == 2) next_candidate = 3;
        else           next_candidate += 2;

        if (!is_prime_big((seq_t)cand)) continue;   /* skip non‑primes */

        if (is_deletable((seq_t)cand)) {
            /* Store the new deletable prime */
            if (terms_len == terms_cap) {
                size_t newcap = terms_cap ? terms_cap * 2 : 8;
                terms = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
                terms_cap = newcap;
            }
            terms[terms_len++] = (seq_t)cand;
        }
    }
    return terms[n - 1];
}
