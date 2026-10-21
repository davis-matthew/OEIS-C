/*
 * A285687  Lexicographically earliest sequence of distinct positive terms
 *          such that, for any n>0, n and a(n) differ and overlap in base 10
 *          (in the sense of A262323).
 * https://oeis.org/A285687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 135, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A285687
 * Lexicographically earliest self‑inverse permutation of the positive integers
 * such that n ≠ a(n) and n overlaps a(n) in base 10.
 * Computation: greedy pairing – for each unpaired i choose the smallest
 * unused q that overlaps with i, then set a(i)=q and a(q)=i.
 * The function memoises results, keeping a static array that grows as
 * necessary.  It works for any order of calls.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/digits.h"   /* provides: int digits(idx_t n) */

static idx_t pow10arr[21];          /* 10^0 .. 10^20 (fits in 64‑bit) */
static bool pow10_inited = false;
static void init_pow10(void) {
    if (pow10_inited) return;
    pow10arr[0] = 1;
    for (int i = 1; i <= 20; ++i) pow10arr[i] = pow10arr[i - 1] * 10ull;
    pow10_inited = true;
}

/* Test whether a and b overlap (suffix of one is prefix of the other). */
static bool overlaps(idx_t a, idx_t b) {
    int da = digits(a);
    int db = digits(b);
    int m  = da < db ? da : db;
    for (int k = 1; k <= m; ++k) {
        idx_t powk    = pow10arr[k];
        idx_t powda_k = pow10arr[da - k];
        idx_t powdb_k = pow10arr[db - k];
        if ((a % powk) == (b / powdb_k)) return true;
        if ((b % powk) == (a / powda_k)) return true;
    }
    return false;
}

/* Dynamic arrays for the permutation and used flags. */
static seq_t *perm   = NULL;   /* 0‑based, index 0 unused      */
static bool *used   = NULL;
static idx_t capacity = 0;   /* highest index the arrays cover (0‑based) */
static idx_t processed = 0;  /* all indices <= processed have been examined */

/* Expand the arrays to cover at least index mx (1‑based). */
static void ensure_capacity(idx_t mx) {
    if (mx <= capacity) return;
    idx_t newcap = capacity ? capacity << 1 : 128;
    while (newcap < mx) newcap <<= 1;
    perm = (seq_t *)realloc(perm, sizeof(seq_t) * (newcap + 1));
    used = (bool  *)realloc(used, sizeof(bool)  * (newcap + 1));
    /* Mark new entries as unused; perm entries remain uninitialised. */
    for (idx_t i = capacity + 1; i <= newcap; ++i) used[i] = false;
    capacity = newcap;
}

/* Main function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;      /* safety for offset   */
    init_pow10();

    ensure_capacity(n);

    /* Build the permutation up to n if not finished. */
    while (processed < n) {
        idx_t i = processed + 1;

        if (used[i]) { /* already paired earlier */
            processed = i;
            continue;
        }

        /* Find the smallest unused q that overlaps i. */
        for (idx_t q = 1; ; ++q) {
            if (q == i) continue;
            if (q <= capacity && used[q]) continue;
            if (overlaps(i, q)) {
                if (q > capacity) ensure_capacity(q);
                /* Assign pair */
                perm[i] = (seq_t)q;
                perm[q] = (seq_t)i;
                used[i] = true;
                used[q] = true;
                break;
            }
        }
        processed = i;
    }

    return perm[n];
}
