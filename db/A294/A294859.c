/*
 * A294859  Triangle whose n-th row is the concatenated sequence of all Lyndon
 *          compositions of n in lexicographic order.
 * https://oeis.org/A294859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes a(n) by generating, for each integer, all Lyndon
 * compositions in lexicographic order.  The rows are concatenated
 * element‑wise and the nth element of this infinite sequence is returned.
 * For each row the recursive generator produces every composition of the
 * target weight; those that are Lyndon (primitive and lexicographically
 * smallest among their rotations) are counted.  The overall running time
 * is dominated by the enumeration of at most 2^(k-1) compositions for
 * the current row k (k≈20 for n≤10000), well below the 2 s limit.
 */
#include <stdbool.h>
#include <stddef.h>

/* Determine if w[0..len-1] is a Lyndon word (primitive and 
 * lexicographically smallest among all its cyclic shifts).         */
static bool is_lyndon_vec(const int *w, idx_t len)
{
    /* Primitive check: no proper period divides the word. */
    for (idx_t d = 1; d * 2 <= len; ++d) {
        if (len % d != 0) continue;
        bool same = true;
        for (idx_t i = 0; i < len; ++i) {
            if (w[i] != w[i % d]) { same = false; break; }
        }
        if (same) return false;      /* periodic */
    }

    /* Lexicographic minimality: w < every nontrivial rotation. */
    for (idx_t shift = 1; shift < len; ++shift) {
        for (idx_t i = 0; i < len; ++i) {
            int a = w[i];
            int b = w[(i + shift) % len];
            if (a < b) break;           /* rotation is greater */
            if (a > b) return false;    /* rotation is smaller */
        }
    }
    return true;
}

/* Recursive generation of all compositions of 'target'.
 * cur holds the current partial composition (length 'len').
 * When a complete composition equals 'target', it is tested for
 * the Lyndon property; if it is Lyndon we either return the desired
 * element or subtract its length from 'rem'.
 */
static void rec_lnds(idx_t target, idx_t sum, int *cur, idx_t len,
                     idx_t *rem, seq_t *out, bool *found)
{
    if (*found) return;

    if (sum == target) {
        if (is_lyndon_vec(cur, len)) {
            if (*rem <= len) {
                *out = (seq_t)cur[*rem - 1];
                *found = true;
            } else {
                *rem -= len;
            }
        }
        return;
    }

    for (idx_t part = 1; part <= target - sum; ++part) {
        cur[len] = (int)part;
        rec_lnds(target, sum + part, cur, len + 1, rem, out, found);
        if (*found) return;
    }
}

/* Main sequence function.  n is 1‑based. */
seq_t a(idx_t n)
{
    idx_t rem = n;          /* remaining index to find */
    seq_t result = 0;       /* will hold the found value */

    for (idx_t row = 1; ; ++row) {
        int cur[128];          /* buffer for a composition (row ≤ 128) */
        bool found = false;
        rec_lnds(row, 0, cur, 0, &rem, &result, &found);
        if (found) return result;
        /* otherwise rem has been reduced by the entire row length
           and the loop continues with the next row.            */
    }
}
