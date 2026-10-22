/*
 * A383076  Triangle T(n,k) read by rows: where T(n,k) is the number of the
 *          k-th eliminated person in the variation of the Josephus
 *          elimination process for n people, where the elimination pattern is
 *          eliminate-skip-eliminate.
 * https://oeis.org/A383076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Compute the flattened triangle T(n,k) for the Josephus
   elimination variant “eliminate‑skip‑eliminate”.
   For a given flattened index n, locate the row r such that
   1 + 2 + … + (r‑1) < n ≤ 1 + 2 + … + r, compute the (n‑cum)-th
   elimination within that row by simulation, and return the person
   eliminated at that position. */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* locate the row containing the n‑th element */
    idx_t r = 1;
    idx_t cum = 0;           /* sum of rows before r */
    while (cum + r < n) {    /* find first r with cum + r >= n */
        cum += r;
        r++;
    }
    idx_t k = n - cum;       /* 1‑based position inside row r */

    /* simulate elimination process for row r until the k‑th elimination */
    idx_t *rem = (idx_t *)malloc((size_t)r * sizeof(idx_t));
    for (idx_t i = 0; i < r; ++i) rem[i] = i + 1;

    idx_t count = r;         /* number of people remaining */
    idx_t pos = 0;           /* current pointer in rem */
    idx_t elim_cnt = 0;      /* number of eliminations performed */
    seq_t result = 0;        /* last eliminated person (to return) */

    while (count > 0) {
        /* eliminate current person */
        result = (seq_t)rem[pos];
        elim_cnt++;
        if (elim_cnt == k) { free(rem); return result; }

        memmove(rem + pos, rem + pos + 1,
                (size_t)(count - pos - 1) * sizeof(idx_t));
        count--;
        if (count == 0) break;
        pos %= count;        /* adjust after removal */

        /* skip one person */
        pos = (pos + 1) % count;

        /* eliminate the skipped person */
        result = (seq_t)rem[pos];
        elim_cnt++;
        if (elim_cnt == k) { free(rem); return result; }

        memmove(rem + pos, rem + pos + 1,
                (size_t)(count - pos - 1) * sizeof(idx_t));
        count--;
        if (count == 0) break;
        pos %= count;
    }

    free(rem);
    return result;          /* should never reach here */
}
