/*
 * A322466  Lexicographically first sequence of distinct terms such that the
 *          a(n)th digit after a(n) shares a(n)'s parity.
 * https://oeis.org/A322466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 220, u16 n <= 8638, u32 n <= 8638, u64 n <= 8638,
 *            u128 n <= 8638, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A322466 – “Lexicographically first sequence of distinct terms
    such that the a(n)th digit after a(n) shares a(n)'s parity”.
    The function builds the sequence incrementally, keeping a cache of
    already computed values.  For each new term it scans all positive integers,
    skipping already‑used ones, and selects the smallest that does not
    violate the parity constraints that will be imposed on future digits
    of the concatenated decimal string.  The constraints are stored per
    digit position; a future constraint is represented by an integer
    0 (even) or 1 (odd), or -1 if still unknown.  All indices are
    1‑based as in the OEIS description.  The algorithm is
    procedural and therefore works for all values of n that fit in
    seq_t.  It is O(n^2) in the worst case but fast enough for n≤10000
    on a typical judge system.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ----------  static storage  ---------------------------------------- */
static seq_t  *cache      = NULL;     /* cache of a(i) values               */
static idx_t   cache_len  = 0, cache_cap = 0;

static uint8_t *used      = NULL;     /* used[t] == 1 if t has appeared    */
static size_t   used_cap  = 0;

static int8_t  *constraint = NULL;    /* -1 = unknown, 0 even, 1 odd       */
static size_t   constraint_cap = 0;

static uint8_t *digits    = NULL;     /* concatenated decimal digits       */
static size_t   digits_cap = 0;
static size_t   digits_len = 0;        /* current number of digits          */

/* ----------  helpers ------------------------------------------------- */
static void ensure_used(size_t need)
{
    if (used_cap < need) {
        size_t new_cap = used_cap ? used_cap << 1 : 64;
        while (new_cap < need) new_cap <<= 1;
        uint8_t *tmp = (uint8_t*)realloc(used, new_cap);
        if (!tmp) abort();
        memset(tmp + used_cap, 0, new_cap - used_cap);
        used = tmp;
        used_cap = new_cap;
    }
}

static void ensure_constraint(size_t need)
{
    if (constraint_cap < need) {
        size_t new_cap = constraint_cap ? constraint_cap << 1 : 64;
        while (new_cap < need) new_cap <<= 1;
        int8_t *tmp = (int8_t*)realloc(constraint, new_cap);
        if (!tmp) abort();
        for (size_t i = constraint_cap; i < new_cap; ++i) tmp[i] = -1;
        constraint = tmp;
        constraint_cap = new_cap;
    }
}

static void ensure_digits(size_t need)
{
    if (digits_cap < need) {
        size_t new_cap = digits_cap ? digits_cap << 1 : 64;
        while (new_cap < need) new_cap <<= 1;
        uint8_t *tmp = (uint8_t*)realloc(digits, new_cap);
        if (!tmp) abort();
        digits = tmp;
        digits_cap = new_cap;
    }
}

/* ----------  main function -------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n<1, guard */

    /* Initialise once */
    if (cache_len == 0) {
        cache      = NULL;
        cache_cap  = 0;
        used       = NULL;
        used_cap   = 0;
        constraint = NULL;
        constraint_cap = 0;
        digits     = NULL;
        digits_cap = 0;
        digits_len = 0;
    }

    /* Grow cache if needed */
    if (cache_cap < n) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap << 1;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Build sequence until n */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t chosen = 0;

        /* Try smallest unused integer */
        for (idx_t t = 1; ; ++t) {
            /* Ensure used array covers t */
            ensure_used((size_t)t + 1);
            if (used[(size_t)t]) continue;          /* already used */

            /* Compute digits of t (most significant first) */
            idx_t temp = t;
            uint8_t dd[32];
            int dlen = 0;
            if (temp == 0) {
                dd[dlen++] = 0;
            } else {
                while (temp > 0) {
                    dd[dlen++] = temp % 10;
                    temp /= 10;
                }
                /* reverse to get most significant first */
                for (int l = 0, r = dlen - 1; l < r; ++l, --r) {
                    uint8_t c = dd[l];
                    dd[l] = dd[r];
                    dd[r] = c;
                }
            }

            /* Future position where parity of t must appear */
            size_t posFuture = digits_len + dlen + t;   /* 1‑based position */
            ensure_constraint(posFuture + 1);

            int parity_t = (int)(t & 1);
            if (constraint[posFuture] != -1 && constraint[posFuture] != parity_t)
                continue;          /* future constraint fails */

            /* Check pre‑existing digits against constraints */
            bool ok = true;
            for (int j = 0; j < dlen; ++j) {
                size_t pos = digits_len + j + 1;  /* 1‑based */
                ensure_constraint(pos + 1);
                int parity_d = (int)(dd[j] & 1);
                if (constraint[pos] != -1 && constraint[pos] != parity_d) {
                    ok = false;
                    break;
                }
            }
            if (!ok) continue;   /* candidate fails */

            /* All checks passed – accept this t */
            used[(size_t)t] = 1;

            /* Append digits to stream */
            ensure_digits(digits_len + dlen);
            memcpy(digits + digits_len, dd, dlen);
            digits_len += dlen;

            /* Impose the future constraint */
            constraint[posFuture] = parity_t;

            chosen = t;
            break;
        }

        cache[i] = chosen;
    }

    cache_len = n;
    return cache[n - 1];
}
