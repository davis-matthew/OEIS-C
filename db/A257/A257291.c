/*
 * A257291  Number of states in minimal DFA accepting base-2 representation of
 *          first n prime numbers.
 * https://oeis.org/A257291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of states in the minimal DFA that accepts * the
 *            base‑2 representations of the first n prime numbers. *
 *            Computation: build a trie of all binary strings of the first n
 *            primes, * then merge identical sub‑automata by recursive
 *            hashing. The result * counts all distinct states, including...
 * Range:     u8 n <= 421, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* ----------  Helper: generate first n primes  ---------- */
static void get_first_n_primes(idx_t n, uint64_t *primes_out)
{
    size_t limit = n * 15 + 100;          /* rough upper bound */
    if (limit < 20) limit = 20;           /* ensure small cases work */

    while (1) {
        bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
        if (!is_prime) exit(1);           /* allocation failure */

        size_t i;
        for (i = 0; i <= limit; ++i) is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;

        for (i = 2; i * i <= limit; ++i) {
            if (is_prime[i]) {
                size_t j;
                for (j = i * i; j <= limit; j += i) {
                    is_prime[j] = false;
                }
            }
        }

        size_t count = 0;
        for (i = 2; i <= limit && count < n; ++i) {
            if (is_prime[i]) {
                primes_out[count++] = i;
            }
        }

        free(is_prime);

        if (count >= n)
            break;                         /* enough primes found */

        /* increase the trial limit and try again */
        limit *= 2;
    }
}

/* ----------  DFA construction & minimisation  ---------- */
typedef struct {
    int child[2];   /* -1 = no edge yet (points to sink) */
    unsigned accept : 1;
} Node;

typedef struct {
    unsigned acc : 1;
    int c0;
    int c1;
} Key;

/* Recursively compute a canonical ID for each node.
   Nodes with identical (accept, child0, child1) get the same ID. */
static int canon_rec(int u, Node *nodes, int *canon, int *next_id,
                     Key *tbl_keys, int *tbl_ids, size_t tbl_mask)
{
    if (canon[u] != -1)
        return canon[u];

    int c0 = (nodes[u].child[0] == -1) ? -1
                 : canon_rec(nodes[u].child[0], nodes, canon, next_id,
                             tbl_keys, tbl_ids, tbl_mask);
    int c1 = (nodes[u].child[1] == -1) ? -1
                 : canon_rec(nodes[u].child[1], nodes, canon, next_id,
                             tbl_keys, tbl_ids, tbl_mask);

    Key key;
    key.acc = nodes[u].accept;
    key.c0  = c0;
    key.c1  = c1;

    /* Simple deterministic hash */
    unsigned h = key.acc ? 1237u : 1031u;
    h = h * 1000003u + (unsigned)c0;
    h = h * 1000003u + (unsigned)c1;
    size_t idx = h & tbl_mask;

    while (1) {
        if (tbl_ids[idx] == -1) {          /* empty slot → insert */
            tbl_keys[idx] = key;
            tbl_ids[idx] = *next_id;
            canon[u] = *next_id;
            (*next_id)++;
            return tbl_ids[idx];
        }
        if (tbl_keys[idx].acc == key.acc &&
            tbl_keys[idx].c0  == key.c0  &&
            tbl_keys[idx].c1  == key.c1) {  /* found existing key */
            canon[u] = tbl_ids[idx];
            return tbl_ids[idx];
        }
        idx = (idx + 1) & tbl_mask;       /* linear probe */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined below the offset */

    /* ----- 1. Generate the first n primes ----- */
    uint64_t *primes = (uint64_t *)malloc(n * sizeof(uint64_t));
    if (!primes) exit(1);
    get_first_n_primes(n, primes);

    /* ----- 2. Build a trie of binary representations ----- */
    size_t nodes_cap = 1024;
    size_t nodes_used = 1;               /* root at index 0 */
    Node *nodes = (Node *)malloc(nodes_cap * sizeof(Node));
    if (!nodes) exit(1);
    nodes[0].child[0] = nodes[0].child[1] = -1;
    nodes[0].accept = 0;

    for (size_t pi = 0; pi < n; ++pi) {
        uint64_t p = primes[pi];
        /* find most significant bit */
        int msb = 0;
        uint64_t v = p;
        while (v >> 1) { ++msb; v >>= 1; }

        int cur = 0;
        for (int k = msb; k >= 0; --k) {
            int bit = (p >> k) & 1;
            int nxt = nodes[cur].child[bit];
            if (nxt == -1) {
                if (nodes_used == nodes_cap) {
                    nodes_cap <<= 1;
                    nodes = (Node *)realloc(nodes, nodes_cap * sizeof(Node));
                    if (!nodes) exit(1);
                }
                nxt = nodes_used++;
                nodes[nxt].child[0] = nodes[nxt].child[1] = -1;
                nodes[nxt].accept = 0;
                nodes[cur].child[bit] = nxt;
            }
            cur = nxt;
        }
        nodes[cur].accept = 1;
    }

    /* ----- 3. Canonicalise (minimise) the trie ----- */
    int *canon = (int *)malloc(nodes_used * sizeof(int));
    if (!canon) exit(1);
    for (size_t i = 0; i < nodes_used; ++i)
        canon[i] = -1;

    size_t tbl_size = 1;
    while (tbl_size < nodes_used * 2) tbl_size <<= 1;
    Key *tbl_keys = (Key *)malloc(tbl_size * sizeof(Key));
    if (!tbl_keys) exit(1);
    int *tbl_ids = (int *)malloc(tbl_size * sizeof(int));
    if (!tbl_ids) exit(1);
    memset(tbl_ids, 0xFF, tbl_size * sizeof(int));   /* -1 */
    size_t tbl_mask = tbl_size - 1;

    int next_id = 0;
    canon_rec(0, nodes, canon, &next_id,
              tbl_keys, tbl_ids, tbl_mask);

    seq_t result = (seq_t)(next_id + 1);  /* +1 for the unique dead sink */

    /* ----- 4. Clean up ----- */
    free(nodes);
    free(primes);
    free(canon);
    free(tbl_keys);
    free(tbl_ids);

    return result;
}
