/*
 * A326753  Number of connected components of the set-system with BII-number
 *          n.
 * https://oeis.org/A326753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   find_set
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A326753: number of connected components of the set‑system with BII‑number n */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/find_set.h"

static void union_set(int *parent, int *rank, int a, int b) {
    int ra = find_set(parent, a);
    int rb = find_set(parent, b);
    if (ra == rb) return;
    if (rank[ra] < rank[rb]) parent[ra] = rb;
    else if (rank[ra] > rank[rb]) parent[rb] = ra;
    else {
        parent[rb] = ra;
        rank[ra]++;
    }
}

seq_t a(idx_t n) {
    /* Special case: empty set‑system */
    if (n == 0) return 0;

    /* Step 1: binary indices (positions of 1 bits) of n */
    uint8_t bin_indices[64];
    size_t idx_cnt = 0;
    {
        uint64_t temp = n;
        uint8_t pos = 1;
        while (temp) {
            if (temp & 1ULL) bin_indices[idx_cnt++] = pos;
            temp >>= 1ULL;
            pos++;
        }
    }

    /* Union‑find over vertices 1…64 (enough for all possible indices) */
    enum { MAXV = 65 };                /* indices 0…64, index 0 unused */
    int parent[MAXV];
    int rankv[MAXV];
    bool used[MAXV];
    for (int i = 0; i < MAXV; i++) {
        parent[i] = i;
        rankv[i] = 0;
        used[i] = false;
    }

    /* Process each binary index of n */
    for (size_t it = 0; it < idx_cnt; it++) {
        uint64_t k = bin_indices[it];

        /* Binary indices of k */
        uint8_t elem[64];
        size_t elem_cnt = 0;
        {
            uint64_t tk = k;
            uint8_t pk = 1;
            while (tk) {
                if (tk & 1ULL) {
                    elem[elem_cnt++] = pk;
                    used[pk] = true;        /* mark this vertex as present */
                }
                tk >>= 1ULL;
                pk++;
            }
        }

        /* Union all vertices inside this set */
        if (elem_cnt >= 2) {
            int root = elem[0];
            for (size_t j = 1; j < elem_cnt; j++)
                union_set(parent, rankv, root, elem[j]);
        }
    }

    /* Count distinct connected components among used vertices */
    bool seen[MAXV];
    for (int i = 0; i < MAXV; i++) seen[i] = false;
    seq_t comps = 0;
    for (int v = 1; v < MAXV; v++) {
        if (!used[v]) continue;
        int r = find_set(parent, v);
        if (!seen[r]) {
            seen[r] = true;
            comps++;
        }
    }

    return comps;
}
