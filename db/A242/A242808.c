/*
 * A242808  The smallest n-digit number where first k digits are divisible by
 *          k-th Fibonacci number for k = 1..n.
 * https://oeis.org/A242808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Validated: full, pass, 11 terms through n = 11 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by depth‑first search constructing the minimal
   n‑digit number whose first k digits are divisible by the k‑th
   Fibonacci number. The sequence is finite; for n>11 it is
   undefined and this implementation returns 0. */
#include <stdbool.h>

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

/* Helper that performs a depth‑first search.
   pos   : current position (1‑based)
   n     : total length
   pref  : prefix constructed so far
   out   : pointer to store the found number (if any)   */
static bool dfs(idx_t n, idx_t pos, seq_t pref, seq_t *out)
{
    if (pos > n) {                /* all positions filled */
        *out = pref;
        return true;
    }
    seq_t f = fib(pos);           /* Fibonacci number F_pos   */
    int start = (pos == 1)? 1: 0; /* first digit cannot be 0 */
    for (int d = start; d <= 9; ++d) {
        seq_t cand = pref * 10 + d;
        if (cand % f == 0) {      /* prefix divisible by F_pos ? */
            if (dfs(n, pos + 1, cand, out))
                return true;
        }
    }
    return false;                 /* no digit fits at this level */
}

/* Compute the smallest n‑digit number satisfying the
   divisibility constraints.  For n>11 the sequence is
   undefined; the function returns 0. */
seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t terms[12];        /* terms[1]..terms[11] are filled once */
    if (!init) {
        for (idx_t i = 1; i <= 11; ++i) {
            seq_t res = 0;
            dfs(i, 1, 0, &res);
            terms[i] = res;
        }
        init = true;
    }
    if (n <= 11)
        return terms[n];
    return (seq_t)0;
}
