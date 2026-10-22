/*
 * A328326  Least multiple of n that contains only the distinct digits of n
 *          and n+1, with each of those digits appearing at least once.
 * https://oeis.org/A328326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least multiple of n that contains only the distinct
 *            digits of n and n+1, with each of those digits appearing at
 *            least once
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 8, u64 n <= 9008,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* n≥1 in the OEIS offset, guard anyway */

    /* --------- gather the distinct digits of n and n+1 ---------- */
    int present[10] = {0};
    {
        uint64_t m = n;
        if (m == 0) present[0] = 1;
        while (m) { present[m % 10] = 1; m /= 10; }

        m = n + 1;
        if (m == 0) present[0] = 1;
        while (m) { present[m % 10] = 1; m /= 10; }
    }

    /* --------- make an array of these digits and a bit mapping ----- */
    int digits[10], bit_of[10];
    int dsize = 0;
    for (int d = 0; d < 10; ++d) bit_of[d] = -1;
    for (int d = 0; d < 10; ++d) {
        if (present[d]) {
            digits[dsize] = d;
            bit_of[d] = dsize;
            ++dsize;
        }
    }

    int masksize = 1 << dsize;          /* 2^dsize */
    int fullmask = masksize - 1;         /* all needed digits present */

    size_t state_count = (size_t)n * (size_t)masksize; /* (residue, mask) pairs */

    /* ---------- allocate BFS data structures (C++ casts) ------------- */
    uint8_t *visited = (uint8_t *)calloc(state_count, sizeof(uint8_t));
    struct Node { uint32_t prev; uint8_t digit; } *pre =
        (struct Node *)malloc(state_count * sizeof(struct Node));
    uint32_t *queue = (uint32_t *)malloc(state_count * sizeof(uint32_t));

    int qhead = 0, qtail = 0;
    const uint32_t NO_PREV = (uint32_t)~0u;

    /* ---------- start states (first non‑zero digit) ----------------- */
    for (int i = 0; i < dsize; ++i) {
        int d = digits[i];
        if (d == 0) continue;                       /* no leading zero */
        int mask = 1 << bit_of[d];
        int residue = d % (int)n;
        int idx = residue * masksize + mask;
        if (!visited[idx]) {
            visited[idx] = 1;
            pre[idx].prev   = NO_PREV;
            pre[idx].digit  = (uint8_t)d;
            queue[qtail++] = (uint32_t)idx;
        }
    }

    /* --------------------- BFS search --------------------- */
    while (qhead < qtail) {
        uint32_t cur_idx = queue[qhead++];
        int residue = cur_idx / masksize;
        int mask    = cur_idx % masksize;

        /* goal reached? all digits present and remainder zero */
        if (residue == 0 && mask == fullmask) {
            /* reconstruct the digit string */
            int len = 0;
            uint32_t p = cur_idx;
            while (true) {
                ++len;
                uint32_t pp = pre[p].prev;
                if (pp == NO_PREV) break;
                p = pp;
            }

            char *num = (char *)malloc(len + 1);
            num[len] = '\0';
            p = cur_idx;
            int pos = len - 1;
            while (true) {
                num[pos--] = '0' + pre[p].digit;
                uint32_t pp = pre[p].prev;
                if (pp == NO_PREV) break;
                p = pp;
            }

            seq_t result = seq_from_str(num);

            free(num);
            free(visited);
            free(pre);
            free(queue);
            return result;
        }

        /* expand to next digit */
        for (int i = 0; i < dsize; ++i) {
            int d = digits[i];
            int new_residue = (int)((residue * 10ULL + (uint64_t)d) % (uint64_t)n);
            int new_mask = mask | (1 << bit_of[d]);
            int new_idx = new_residue * masksize + new_mask;
            if (!visited[new_idx]) {
                visited[new_idx] = 1;
                pre[new_idx].prev   = cur_idx;
                pre[new_idx].digit  = (uint8_t)d;
                queue[qtail++] = (uint32_t)new_idx;
            }
        }
    }

    /* No solution should theoretically ever occur */
    free(visited);
    free(pre);
    free(queue);
    return 0;
}
