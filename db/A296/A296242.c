/*
 * A296242  Slideable numbers: each digit d can be displaced by d positions
 *          either to the left or to the right, without creating a hole or an
 *          "overlap".
 * https://oeis.org/A296242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 1537, u32 n <= 1933, u64 n <= 1933,
 *            u128 n <= 1933, big any n
 * Validated: standard, pass, 81 terms through n = 1862 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296242: slideable numbers
 * A slideable number is a decimal integer where each digit d can be displaced
 * by exactly d positions either to the left or to the right, without
 * creating any holes or overlaps.  The resulting positions of the digits
 * must form a contiguous block of slots.
 *
 * The sequence starts with: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 13, 20, 22, 24, ...
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

/* ------------------------------------------------------------ */
static bool is_slideable(seq_t n)
{
    /* Extract digits into an array left‑to‑right */
    uint8_t rev[20];   /* digits in reverse order (least significant first) */
    size_t len = 0;

    if (n == (seq_t)0) {
        rev[len++] = 0;
    } else {
        seq_t tmp = n;
        while (tmp != (seq_t)0) {
            rev[len++] = (uint8_t)(tmp % (seq_t)10);
            tmp /= (seq_t)10;
        }
    }

    uint8_t dig[20];
    for (size_t i = 0; i < len; ++i)
        dig[i] = rev[len - 1 - i];   /* reverse to left‑to‑right order */

    /* For small numbers of digits, brute‑force all 2^len direction sets */
    const size_t max_mask = ((size_t)1 << len);
    for (size_t mask = 0; mask < max_mask; ++mask) {
        long long minpos = LLONG_MAX, maxpos = LLONG_MIN;
        long long pos[20];
        bool ok = true;

        for (size_t i = 0; i < len; ++i) {
            int dir = (mask & ((size_t)1 << i)) ? +1 : -1;   /* +1: right, -1: left */
            long long y = (long long)i + (long long)dir * (long long)dig[i];
            pos[i] = y;
            if (y < minpos) minpos = y;
            if (y > maxpos) maxpos = y;
        }

        /* Must occupy a contiguous block of size len */
        if ((size_t)(maxpos - minpos + 1) != len) continue;

        /* Check all positions are distinct */
        for (size_t i = 0; i < len && ok; ++i) {
            for (size_t j = i + 1; j < len; ++j) {
                if (pos[i] == pos[j]) { ok = false; break; }
            }
        }

        if (ok) return true;
    }
    return false;
}

/* Maximum cache size – sufficient for indices up to about 1e5 */
#define MAX_CACHE 200000

/* Internal cache of computed terms */
static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;
static idx_t next_candidate = 0;   /* next integer to test */

/* ------------------------------------------------------------ */
/* Test whether a given number is slideable                     */

/* ------------------------------------------------------------ */
/* External entry point: a(n)                                  */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Ensure cache has at least n terms */
    while (cache_len < (size_t)n && next_candidate < (idx_t)MAX_CACHE) {
        seq_t cur = (seq_t)next_candidate;
        if (is_slideable(cur))
            cache[cache_len++] = cur;
        ++next_candidate;
    }

    /* In the unlikely event that more terms are needed than cache size */
    /* (which should not happen for the intended use), fall back to
       brute force without caching. */
    if (cache_len < (size_t)n) {
        idx_t cand = next_candidate;
        while (cache_len < (size_t)n) {
            seq_t cur = (seq_t)cand;
            if (is_slideable(cur))
                cache[cache_len++] = cur;
            ++cand;
        }
        next_candidate = cand;
    }

    return cache[n - 1];
}
