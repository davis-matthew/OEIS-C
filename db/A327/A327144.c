/*
 * A327144  Spanning edge-connectivity of the set-system with BII-number n.
 * https://oeis.org/A327144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   computed by brute‑force subset enumeration of the set‑system
 *            edges
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the positions (1-based, from the least significant bit) of
       the 1‑bits of n.  These positions are the binary indices of n. */
    int pos_arr[64];
    int e = 0;
    idx_t temp = n;
    int pos = 1;             /* position counter, 1‑based */
    while (temp != 0) {
        if (temp & 1ULL)
            pos_arr[e++] = pos;
        temp >>= 1ULL;
        ++pos;
    }

    /* If there are no binary indices the set‑system is empty. */
    if (e == 0)
        return (seq_t)0;

    /* For each binary index i, build the set of binary indices of i.
       Store each set as a bit mask of its vertices. */
    uint64_t edges[64];
    uint64_t all_vertices = 0;
    for (int k = 0; k < e; ++k) {
        idx_t i = pos_arr[k];
        uint64_t mask = 0;
        idx_t t = i;
        int p = 1;                 /* vertex positions are 1‑based */
        while (t != 0) {
            if (t & 1ULL)
                mask |= 1ULL << (p - 1);
            t >>= 1ULL;
            ++p;
        }
        edges[k] = mask;
        all_vertices |= mask;
    }

    /* Brute‑force search over subsets of edges kept. */
    idx_t min_removed = e;          /* worst case: all edges removed */
    idx_t limit = ((idx_t)1 << e);  /* e ≤ 64 for the intended usage */

    for (idx_t keep = 0; keep < limit; ++keep) {
        uint64_t cov_mask = 0;
        idx_t kept_cnt = 0;
        for (int k = 0; k < e; ++k) {
            if (keep & ((idx_t)1 << k)) {
                cov_mask |= edges[k];
                ++kept_cnt;
            }
        }

        /* If the remaining system covers fewer vertices, property holds. */
        if (cov_mask != all_vertices) {
            idx_t removed = e - kept_cnt;
            if (removed < min_removed)
                min_removed = removed;
            continue;
        }

        /* Connectivity test on the hypergraph of the remaining edges. */
        if (cov_mask == 0)          /* would not happen when all_vertices≠0 */
            continue;

        uint64_t visited = 0;
        unsigned first = __builtin_ctzll(cov_mask);  /* lowest set vertex */
        visited |= 1ULL << first;

        bool changed;
        do {
            changed = false;
            for (int k = 0; k < e; ++k) {
                if (keep & ((idx_t)1 << k)) {
                    uint64_t edge = edges[k];
                    if (edge & visited) {
                        uint64_t new_v = edge & ~visited;
                        if (new_v) {
                            visited |= new_v;
                            changed = true;
                        }
                    }
                }
            }
        } while (changed);

        if (visited != cov_mask) {   /* disconnected */
            idx_t removed = e - kept_cnt;
            if (removed < min_removed)
                min_removed = removed;
        }
    }

    return (seq_t)min_removed;
}
