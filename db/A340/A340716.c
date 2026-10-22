/*
 * A340716  Lexicographically earliest sequence of positive integers with as
 *          many distinct values as possible such that for any n > 0, a(n +
 *          pi(n)) = a(n) (where pi(n) = A000720(n) corresponds to the number
 *          of prime numbers <= n).
 * https://oeis.org/A340716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 1866, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A340716: lexicographically earliest sequence with a(n+pi(n)) = a(n) and distinct
   values for distinct orbits.  The sequence is obtained by building the graph
   (n , n+pi(n)), computing its connected components, and assigning the smallest
   unused positive integer to each component in increasing order of first appearance. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Function prototype supplied by the build.  The types seq_t and idx_t are
   defined by the build system. */
seq_t a(idx_t n);

/* ---------- Implementation details ---------- */

#define LIMIT_CONST 200000UL  /* Sufficient for all n <= 10000 */

static bool initialized = false;

/* Arrays used during initialization */
static idx_t *pi_arr       = NULL; /* prime counting function */
static idx_t *parent_arr   = NULL; /* DSU parent */
static uint8_t *rank_arr   = NULL; /* DSU rank */
static idx_t *root_to_id   = NULL; /* mapping from component root to component ID */
static idx_t *comp_id      = NULL; /* final component IDs for all indices */

/* Union‑Find helpers */
static inline idx_t dsu_find(idx_t x) {
    while (parent_arr[x] != x) {
        parent_arr[x] = parent_arr[parent_arr[x]];
        x = parent_arr[x];
    }
    return x;
}

static inline void dsu_union(idx_t a, idx_t b) {
    idx_t ra = dsu_find(a);
    idx_t rb = dsu_find(b);
    if (ra == rb) return;
    if (rank_arr[ra] < rank_arr[rb]) {
        parent_arr[ra] = rb;
    } else if (rank_arr[ra] > rank_arr[rb]) {
        parent_arr[rb] = ra;
    } else {
        parent_arr[rb] = ra;
        rank_arr[ra]++;
    }
}

/* Initialization function – builds pi, DSU, and component IDs */
static void init(void) {
    const idx_t LIMIT = LIMIT_CONST;
    pi_arr     = (idx_t *)malloc((LIMIT + 1) * sizeof(idx_t));
    parent_arr = (idx_t *)malloc((LIMIT + 1) * sizeof(idx_t));
    rank_arr   = (uint8_t *)calloc(LIMIT + 1, sizeof(uint8_t));
    root_to_id = (idx_t *)calloc(LIMIT + 1, sizeof(idx_t));
    comp_id    = (idx_t *)malloc((LIMIT + 1) * sizeof(idx_t));

    /* Prime sieve to compute pi(n) */
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    for (idx_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t p = 2; p * p <= LIMIT; ++p) {
        if (is_prime[p]) {
            for (idx_t mult = p * p; mult <= LIMIT; mult += p)
                is_prime[mult] = false;
        }
    }
    pi_arr[0] = 0;
    for (idx_t i = 1; i <= LIMIT; ++i)
        pi_arr[i] = pi_arr[i - 1] + (is_prime[i] ? 1 : 0);

    /* Initialize DSU */
    for (idx_t i = 0; i <= LIMIT; ++i) {
        parent_arr[i] = i;
    }

    /* Build equivalence graph edges: n --> n + pi(n) */
    for (idx_t n = 1; n <= LIMIT; ++n) {
        idx_t succ = n + pi_arr[n];
        if (succ <= LIMIT) dsu_union(n, succ);
    }

    /* Assign component IDs in lexicographic order of first appearance */
    idx_t next_id = 1;
    for (idx_t i = 1; i <= LIMIT; ++i) {
        idx_t r = dsu_find(i);
        if (root_to_id[r] == 0) root_to_id[r] = next_id++;
        comp_id[i] = root_to_id[r];
    }

    free(is_prime);
    initialized = true;
}

/* Public interface */
seq_t a(idx_t n) {
    if (!initialized) init();
    if (n == 0) return (seq_t)0;          /* undefined for n < 1 */
    if (n > LIMIT_CONST) return (seq_t)0;   /* out of precomputed range */
    return (seq_t)comp_id[n];
}
