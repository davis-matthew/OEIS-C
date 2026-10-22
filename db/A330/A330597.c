/*
 * A330597  Squarefree numbers in which all substrings are squarefree.
 * https://oeis.org/A330597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 1690, u64 n <= 1690,
 *            u128 n <= 1690, big any n
 * Helpers:   is_squarefree, seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A330597: numbers whose every decimal substring is square‑free. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_squarefree.h"
#include "../utils/seq_cmp.h"

#define MAX_TERMS 2000   /* a(1690) = 77377717, but a small over‑estimate is safe. */

/* Cache for the sequence values. */
static seq_t  _seq[MAX_TERMS];
static size_t _seq_len = 0;
static bool   _seq_ready = false;

/* Absolute maximum value representable by seq_t, promoted to uint64_t for
   intermediate calculations. */
static const uint64_t MAX_VAL = (uint64_t)(~(seq_t)0);

/* Depth‑first search that builds all admissible numbers digit by digit.
   Parameters:
     val : current number (in uint64_t to avoid overflow when seq_t is small)
     len : number of digits already chosen
     suf : table of suffix values, suf[l] = value of the suffix of length l
*/
static void dfs(uint64_t val, uint8_t len, uint64_t *suf)
{
    /* Record the number if it fits in seq_t.  Numbers are generated in
       lexicographic order, but we sort later. */
    if (len > 0 && val <= MAX_VAL) {
        if (_seq_len < MAX_TERMS) {
            _seq[_seq_len++] = (seq_t)val;
        }
    }

    /* Stop if the number is too large to fit in seq_t. */
    if (val > MAX_VAL) {
        return;
    }

    /* The longest known term has 8 digits; pruning on digit count keeps
       recursion bounded. */
    if (len >= 8) {
        return;
    }

    /* Try to append each admissible digit (1,2,3,5,6,7).  Digits 0,4,8,9 are
       forbidden because their single‑digit substrings are not square‑free. */
    for (int d = 1; d <= 9; ++d) {
        if (d == 4 || d == 8 || d == 9) {
            continue;
        }

        uint64_t new_val = val * 10 + d;
        if (new_val > MAX_VAL) { /* would not fit in seq_t */
            continue;
        }

        uint64_t new_suf[12];
        new_suf[1] = d;                   /* suffix of length 1 */
        bool ok = true;

        /* Build longer suffixes and test them for square‑freeness. */
        for (int l = len; l >= 1 && ok; --l) {
            uint64_t s = suf[l] * 10 + d;
            if (s > MAX_VAL) {
                ok = false;                /* suffix too large */
                break;
            }
            if (!is_squarefree((seq_t)s)) {
                ok = false;
                break;
            }
            new_suf[l + 1] = s;
        }

        if (!ok) {
            continue;
        }

        dfs(new_val, len + 1, new_suf);
    }
}

/* Build the entire sequence once, sort it, and set the ready flag. */
static void generate_all(void)
{
    if (_seq_ready) {
        return;
    }

    /* Begin with each valid one‑digit prefix. */
    uint64_t suf[12];
    for (int d = 1; d <= 9; ++d) {
        if (d == 4 || d == 8 || d == 9) {
            continue;
        }
        suf[1] = d;
        dfs((uint64_t)d, 1, suf);
    }

    /* Sort the terms in ascending order. */
    qsort(_seq, _seq_len, sizeof(seq_t), seq_cmp);
    _seq_ready = true;
}

/* Public function as required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                      /* undefined for indices below 1 */
    }

    generate_all();

    size_t idx = (size_t)(n - 1);
    if (idx >= _seq_len) {
        return 0;                      /* out of range (sequence ends at a(1690)) */
    }

    return _seq[idx];
}
