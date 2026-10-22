/*
 * A371452  Number of connected components of the prime indices of the binary
 *          indices of n.
 * https://oeis.org/A371452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of connected components in the graph whose
 *            vertices are the binary indices of n (positions of 1 in the
 *            reversed binary expansion) and the prime indices of those binary
 *            indices. Edges join a binary‑index vertex to a prime‑index
 *            vertex whenever that prime index divides the binary inde...
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Prime values up to 64 and their 1‑based indices. */
static const int PRIME_VALUES[18] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61
};
#define PRIME_COUNT 18

/* Mapping from prime value to its 1‑based index. */
static int PRIME_INDEX_OF[65];   /* 0 if not prime, else index (1‑based) */
static bool PRIMES_INITIALIZED = false;

/* Union–Find (DSU) helpers ------------------------------------------------- */
#define DSU_SIZE (64 + PRIME_COUNT)   /* 64 binary indices + prime nodes */

static inline int dsu_find(int v, int parent[DSU_SIZE]) {
    while (parent[v] != v) {
        parent[v] = parent[parent[v]];
        v = parent[v];
    }
    return v;
}

static inline void dsu_union(int a, int b,
                             int parent[DSU_SIZE], int rankv[DSU_SIZE]) {
    int ra = dsu_find(a, parent);
    int rb = dsu_find(b, parent);
    if (ra == rb) return;
    if (rankv[ra] < rankv[rb]) {
        int t = ra; ra = rb; rb = t;
    }
    parent[rb] = ra;
    if (rankv[ra] == rankv[rb]) rankv[ra]++;
}

/* Initialize PRIME_INDEX_OF ------------------------------------------------ */
static void init_primes(void) {
    if (PRIMES_INITIALIZED) return;
    memset(PRIME_INDEX_OF, 0, sizeof(PRIME_INDEX_OF));
    for (int i = 0; i < PRIME_COUNT; ++i) {
        int p = PRIME_VALUES[i];
        PRIME_INDEX_OF[p] = i + 1;   /* indices start at 1 */
    }
    PRIMES_INITIALIZED = true;
}

/* Main sequence function --------------------------------------------------- */
seq_t a(idx_t n) {
    init_primes();

    /* DSU structure: 0..63 for binary indices, 64..63+PRIME_COUNT-1 for prime indices. */
    int parent[DSU_SIZE];
    int rankv[DSU_SIZE];
    for (int i = 0; i < DSU_SIZE; ++i) {
        parent[i] = i;
        rankv[i] = 0;
    }

    static bool binary_present[64];
    static bool prime_present[PRIME_COUNT];
    memset(binary_present, 0, sizeof(binary_present));
    memset(prime_present, 0, sizeof(prime_present));

    /* Scan bits of n (positions 0–63). */
    for (idx_t pos = 0; pos < 64 && (n >> pos); ++pos) {
        if ((n >> pos) & 1ULL) {
            binary_present[pos] = true;
            int bin_idx = (int)pos + 1;          /* binary index (1‑based) */

            /* Factor bin_idx using precomputed primes <= 64. */
            int temp = bin_idx;
            for (int i = 0; i < PRIME_COUNT && PRIME_VALUES[i] * PRIME_VALUES[i] <= temp; ++i) {
                int p = PRIME_VALUES[i];
                if (temp % p == 0) {
                    int k = PRIME_INDEX_OF[p];   /* prime index of p (1‑based) */
                    if (k > 0) {
                        dsu_union((int)pos,
                                  64 + (k - 1),
                                  parent, rankv);
                        prime_present[k - 1] = true;
                    }
                    while (temp % p == 0) temp /= p;
                }
            }
            if (temp > 1) {  /* remaining prime factor > sqrt(bin_idx) */
                int k = PRIME_INDEX_OF[temp];
                if (k > 0) {
                    dsu_union((int)pos,
                              64 + (k - 1),
                              parent, rankv);
                    prime_present[k - 1] = true;
                }
            }
        }
    }

    /* Count distinct component roots among present vertices. */
    static bool root_seen[DSU_SIZE];
    memset(root_seen, 0, sizeof(root_seen));
    int comp = 0;

    for (int pos = 0; pos < 64; ++pos) {
        if (binary_present[pos]) {
            int r = dsu_find(pos, parent);
            if (!root_seen[r]) {
                root_seen[r] = true;
                comp++;
            }
        }
    }
    for (int k = 0; k < PRIME_COUNT; ++k) {
        if (prime_present[k]) {
            int node = 64 + k;
            int r = dsu_find(node, parent);
            if (!root_seen[r]) {
                root_seen[r] = true;
                comp++;
            }
        }
    }

    return (seq_t)comp;
}
