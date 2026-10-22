/*
 * A356976  Least positive integer m such that the numbers k^3 + 3*k (k =
 *          1..n) are pairwise distinct modulo m.
 * https://oeis.org/A356976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = least positive integer m such that 
 *         f(k) = k^3 + 3k (k = 1..n) are pairwise distinct modulo m.
 *
 * Implementation: Brute-force search over m, using a tag array to test
 *                 distinctness without clearing each time.
 */
#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are provided by the build; do not define them. */

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 1; return 1 for safety on n == 0. */
    if (n == 0)
        return 1;

    /* Upper bound used for the tag array; conjecture guarantees a(n) ≤ 3n. */
    idx_t maxM = 3 * n + 1;
    int *used = (int *)calloc((size_t)maxM, sizeof(int));
    if (!used)
        return 0;               /* allocation failure (should not happen) */

    int tag = 1;               /* current trial identifier */

    for (seq_t m = 1;; ++m) {
        /* Ensure the used array is large enough for the current m. */
        if (m > (seq_t)maxM) {
            size_t newCap = (size_t)m + 1;
            int *tmp = (int *)realloc(used, newCap * sizeof(int));
            if (!tmp) { free(used); return 0; }
            /* Zero the newly allocated portion. */
            for (size_t i = maxM; i < newCap; ++i) tmp[i] = 0;
            used = tmp;
            maxM = newCap;
        }

        bool ok = true;

        /* Test distinctness of f(k) modulo m for k = 1..n. */
        for (idx_t k = 1; k <= n; ++k) {
            seq_t km  = k % m;               /* k   mod m */
            seq_t km2 = (km * km) % m;       /* k^2 mod m */
            seq_t km3 = (km2 * km) % m;      /* k^3 mod m */
            seq_t fk  = (km3 + 3 * km) % m;  /* f(k) mod m */

            int idx = (int)fk;               /* 0 ≤ idx < m ≤ maxM */

            if (used[idx] == tag) {
                ok = false;                  /* duplicate residue */
                break;
            }
            used[idx] = tag;                 /* mark residue seen */
        }

        if (ok) {
            free(used);
            return m;                        /* least m found */
        }

        ++tag;  /* next trial will use a new tag value */
    }
}
