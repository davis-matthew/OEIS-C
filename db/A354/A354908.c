/*
 * A354908  Numbers k such that the k-th composition in standard order (graded
 *          reverse-lexicographic, A066099) is collapsible.
 * https://oeis.org/A354908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that the k‑th composition (in graded
 *            reverse‑lexicographic order) is collapsible. The function
 *            enumerates k, tests collapsibility, and returns the n‑th such k
 * Range:     u8 n <= 52, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   comp_from_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/comp_from_k.h"

/* Depth‑first search for collapsibility.  No memoisation – the state space is
   small (max 20 parts).  The recursion depth is at most 19. */
static bool dfs(const int *parts, int len)
{
    if (len == 1)
        return true;
    /* For each maximal run of equal parts */
    for (int i = 0; i < len; ++i) {
        int val = parts[i];
        int j = i;
        while (j + 1 < len && parts[j + 1] == val)
            ++j;
        int run_len = j - i + 1;
        if (run_len < 2)
            continue;
        /* Try merging any contiguous sub‑run of this run */
        for (int L = 2; L <= run_len; ++L) {
            for (int start = i; start + L - 1 <= j; ++start) {
                int new_len = len - (L - 1);
                int new_parts[64];
                int idx = 0;
                for (int k = 0; k < start; ++k)
                    new_parts[idx++] = parts[k];
                new_parts[idx++] = val * L;          /* merged value */
                for (int k = start + L; k < len; ++k)
                    new_parts[idx++] = parts[k];
                if (dfs(new_parts, new_len))
                    return true;
            }
        }
    }
    return false;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for offset‑1 sequences */

    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        int parts[64];
        int len;
        comp_from_k(k, parts, &len);
        if (dfs(parts, len)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
