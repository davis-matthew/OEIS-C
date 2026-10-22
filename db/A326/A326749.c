/*
 * A326749  BII-numbers of connected set-systems.
 * https://oeis.org/A326749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 113, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the n-th nonnegative integer m such that the
 * set‑system derived from m via the BII‑number mapping is connected.
 * The function enumerates m in increasing order and returns the n‑th
 * such m.
 */

#include <stdbool.h>
#include <stdint.h>

/* Helper: test whether the set‑system implied by integer v is connected.
 * For v=0 the empty set‑system is considered connected. */
static bool is_connected(idx_t v)
{
    /* Special case: empty set‑system */
    if (v == 0)
        return true;

    /* Collect masks: for each bit position j with bit 1 in v,
     * the corresponding set mask is (j+1). */
    int masks[64];
    int k = 0;
    unsigned long long tmp = (unsigned long long)v;
    while (tmp != 0) {
        int pos = __builtin_ctzll(tmp);       /* position of lowest 1‑bit (0‑based) */
        masks[k++] = pos + 1;                  /* mask index = bit position + 1 */
        tmp &= tmp - 1;                        /* clear lowest 1‑bit */
    }

    /* BFS over the graph of sets (indices of masks).
     * Two sets are adjacent if their masks intersect. */
    bool visited[64] = {false};
    int queue[64];
    int qhead = 0, qtail = 0;

    visited[0] = true;
    queue[qtail++] = 0;

    while (qhead < qtail) {
        int u = queue[qhead++];
        unsigned long long mask_u = (unsigned long long)masks[u];
        for (int v_idx = 0; v_idx < k; ++v_idx) {
            if (!visited[v_idx]) {
                unsigned long long mask_v = (unsigned long long)masks[v_idx];
                if (mask_u & mask_v) {
                    visited[v_idx] = true;
                    queue[qtail++] = v_idx;
                }
            }
        }
    }

    /* If all sets were visited, the set‑system is connected. */
    for (int i = 0; i < k; ++i) {
        if (!visited[i])
            return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 0;
    while (1) {
        if (is_connected(m)) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
        ++m;
    }
}
