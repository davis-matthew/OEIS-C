/*
 * A334914  Least positive multiple of n that when written in base 10 uses
 *          only 0's, 1's, 2's and 3's.
 * https://oeis.org/A334914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 98, u32 n <= 7991, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the least positive multiple of n whose decimal digits are
   only 0, 1, 2 and 3.  The algorithm performs a breadth‑first search
   over the set of remainders modulo n. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0u) return 0;

    /* Allocate arrays sized n; casts required for C++ */
    idx_t *queue  = (idx_t *)malloc(n * sizeof(idx_t));
    idx_t *parent = (idx_t *)malloc(n * sizeof(idx_t));
    uint8_t *digit   = (uint8_t *)malloc(n * sizeof(uint8_t));
    uint8_t *used    = (uint8_t *)malloc(n * sizeof(uint8_t));
    if (!queue || !parent || !digit || !used) {
        free(queue); free(parent); free(digit); free(used);
        return 0;
    }
    for (idx_t i = 0; i < n; ++i) used[i] = 0;

    size_t head = 0, tail = 0;

    /* Initialise queue with admissible one‑digit numbers 1, 2, 3 */
    for (uint8_t d = 1; d <= 3; ++d) {
        idx_t r = d % n;
        if (!used[r]) {
            used[r] = 1;
            parent[r] = n;          /* sentinel for a root node */
            digit[r]  = d;
            queue[tail++] = r;
            if (r == 0) {           /* found immediately */
                head = tail;        /* terminate BFS loop */
                break;
            }
        }
    }

    /* Breadth‑first search through remainders */
    while (head < tail) {
        idx_t cur = queue[head++];
        if (cur == 0) break;        /* found minimal number */
        for (uint8_t d = 0; d <= 3; ++d) {
            idx_t r = ((unsigned long long)cur * 10ULL + (unsigned long long)d) % n;
            if (!used[r]) {
                used[r] = 1;
                parent[r] = cur;
                digit[r]  = d;
                queue[tail++] = r;
                if (r == 0) break;
            }
        }
    }

    /* Reconstruct the minimal number from remainder 0 */
    seq_t value = 0;
    if (used[0]) {
        uint8_t *rev = (uint8_t *)malloc(n * sizeof(uint8_t));
        if (rev) {
            size_t rev_len = 0;
            idx_t r = 0;
            while (true) {
                rev[rev_len++] = digit[r];
                if (parent[r] == n) break;
                r = parent[r];
            }
            for (ptrdiff_t i = (ptrdiff_t)rev_len - 1; i >= 0; --i)
                value = value * 10 + (seq_t)rev[i];
            free(rev);
        }
    }

    free(queue);
    free(parent);
    free(digit);
    free(used);
    return value;
}
