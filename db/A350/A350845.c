/*
 * A350845  Heinz numbers of integer partitions with at least two adjacent
 *          parts of quotient 2.
 * https://oeis.org/A350845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/********************************************************************
 * Formula: a(n) = Heinz number of the partition whose Heinz number
 * is the n‑th smallest among all partitions that contain at least
 * one adjacent pair (y_i , y_{i+1}) with either y_i = 2*y_{i+1}
 * or y_{i+1} = 2*y_i.
 *
 * The algorithm generates all non‑increasing integer tuples
 * (partitions) in increasing order of their Heinz number
 * (product of the primes indexed by the parts), storing only
 * those that satisfy the 2:1 adjacency condition.
 ********************************************************************/
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    seq_t  val;   /* Heinz number of the partial partition */
    idx_t  last;  /* index of the last (smallest) part used so far */
    bool   flag;  /* true iff the current partition already has a 2:1 pair */
} Node;

/* ---------- simple binary min‑heap -------------------------------- */
static void heap_push(Node **heap, size_t *sz, size_t *cap, Node v)
{
    if (*sz + 1 > *cap) {
        *cap = (*cap == 0) ? 16 : (*cap << 1);
        *heap = (Node*)realloc(*heap, *cap * sizeof(Node));
    }
    size_t i = *sz;
    (*heap)[i] = v;
    ++(*sz);
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if ((*heap)[p].val <= (*heap)[i].val) break;
        Node tmp = (*heap)[p];
        (*heap)[p] = (*heap)[i];
        (*heap)[i] = tmp;
        i = p;
    }
}
static Node heap_pop(Node **heap, size_t *sz, size_t *cap)
{
    Node res = (*heap)[0];
    --(*sz);
    (*heap)[0] = (*heap)[*sz];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < *sz && (*heap)[l].val < (*heap)[smallest].val)
            smallest = l;
        if (r < *sz && (*heap)[r].val < (*heap)[smallest].val)
            smallest = r;
        if (smallest == i) break;
        Node tmp = (*heap)[i];
        (*heap)[i] = (*heap)[smallest];
        (*heap)[smallest] = tmp;
        i = smallest;
    }
    return res;
}

/* ---------- prime generation (lazy) -------------------------------- */
static seq_t *primes    = NULL;   /* 0‑based array of primes  */
static idx_t  prime_cnt = 0;      /* number of primes stored      */
static bool   primes_ok = false;
static void init_primes(void)
{
    const idx_t SIEVE_LIMIT = 300000;   /* enough for first ≈10⁴ terms   */
    const idx_t TARGET_CNT  = 20000;    /* store at least this many primes   */
    bool *sieve = (bool*)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    if (!sieve) return;
    size_t cap = 1024;
    primes = (seq_t*)malloc(cap * sizeof(seq_t));
    if (!primes) { free(sieve); return; }
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!sieve[i]) {
            if (prime_cnt == TARGET_CNT) break;
            if (prime_cnt == cap) {
                cap <<= 1;
                primes = (seq_t*)realloc(primes, cap * sizeof(seq_t));
            }
            primes[prime_cnt++] = (seq_t)i;
            for (idx_t j = i + i; j <= SIEVE_LIMIT; j += i)
                sieve[j] = true;
        }
    }
    free(sieve);
}

/* ---------- global state for the heap and the cache -------------- */
static Node *heap   = NULL;
static size_t heap_sz  = 0;
static size_t heap_cap = 0;
static seq_t *cache   = NULL;
static idx_t  cache_len = 0;
static idx_t  cache_cap = 0;

/* ---------- public sequence function ----------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!primes_ok) { init_primes(); primes_ok = true; }

    /* enlarge the cache until it contains at least n elements */
    while (cache_len < n) {
        /* first call – seed the heap with all 1‑part partitions   */
        if (heap_sz == 0 && cache_len == 0) {
            for (idx_t i = 0; i < prime_cnt; ++i) {
                Node v = { primes[i], i + 1, false };
                heap_push(&heap, &heap_sz, &heap_cap, v);
            }
        }

        /* pop the smallest partition so far */
        Node cur = heap_pop(&heap, &heap_sz, &heap_cap);

        /* store only if it already contains a 2:1 adjacent pair */
        if (cur.flag) {
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap == 0) ? 16 : (cache_cap << 1);
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = cur.val;
        }

        /* generate successors by appending a new part j (1 ≤ j ≤ cur.last) */
        for (idx_t j = 1; j <= cur.last; ++j) {
            seq_t prod     = cur.val * primes[j - 1];
            bool   new_flag = cur.flag || (cur.last == 2 * j) || (j == 2 * cur.last);
            Node child = { prod, j, new_flag };
            heap_push(&heap, &heap_sz, &heap_cap, child);
        }
    }
    return cache[n - 1];
}
