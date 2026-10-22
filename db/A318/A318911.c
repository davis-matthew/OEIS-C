/*
 * A318911  Numbers k such that -3 is a quadratic residue modulo 360*k + 1,
 *          360*k + 2, 360*k + 3 and 360*k + 4.
 * https://oeis.org/A318911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 5576, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _good_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A318911: a(n) = k such that -3 is a quadratic residue
 * modulo 360*k+1, 360*k+2, 360*k+3 and 360*k+4.
 * The function computes the terms by incremental search and memoises
 * the results in a static array.  This works for all integer sizes
 * supported by the build (C11 or C++17). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/_good_mod.h"

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Static storage for memoised terms.  Capacity is set large enough
 * for the expected range (up to several hundred thousand k's).      */
#define MAX_TERMS 200000
static seq_t _terms[MAX_TERMS];
static size_t _term_cnt = 0;           /* number of terms stored   */
static uint64_t _tested_k = 0;         /* next k to test           */

/* ------------------------------------------------------------------ */
/* Append a new term to the cache (no dynamic allocation needed).   */
static void _store_term(seq_t value)
{
    if (_term_cnt < (size_t)MAX_TERMS)
        _terms[_term_cnt++] = value;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.                                             */
seq_t a(idx_t n)
{
    /* OEIS uses 1‑based indexing; n >= 1 is guaranteed.            */
    if (n == 0) return 0;  /* defensive, not used in tests */

    /* Generate terms until we reach the required index.            */
    while (_term_cnt < (size_t)n) {
        bool ok = true;
        for (int inc = 1; inc <= 4; ++inc) {
            uint64_t m = 360ULL * _tested_k + (uint64_t)inc;
            if (!_good_mod(m)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            /* Store the current k as the next sequence term.        */
            _store_term((seq_t)_tested_k);
        }
        ++_tested_k;
    }

    return _terms[n - 1];
}
