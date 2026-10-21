/*
 * A283454  The square root of the smallest square referenced in A249025
 *          (Numbers k such that 3^k - 1 is not squarefree).
 * https://oeis.org/A283454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p such that p² | 3^k – 1 where k is the
 *            n‑th integer for which 3^k – 1 is not square‑free. This is
 *            obtained by generating, for each prime p, the arithmetic
 *            progression k = m * ord_{p²}(3) (m ≥ 1) and merging them in
 *            ascending order. For each k the smallest p whose progressio...
 * Range:     u8 n <= 407, u16 n <= 407, u32 n <= 407, u64 n <= 407,
 *            u128 n <= 407, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define LIMIT_PRIME 200000   /* sieve limit – sufficient for n ≤ 10000 */

/* --- modular exponentiation (works for any 0 ≤ base < mod < 2^64) ----- */
static uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1ULL)
            result = (uint64_t)((__uint128_t)result * base % mod);
        base = (uint64_t)((__uint128_t)base * base % mod);
        exp >>= 1;
    }
    return result;
}

/* --- sieve of Eratosthenes, producing all primes ≤ LIMIT_PRIME --------- */
static bool ready = false;
static uint64_t *primes   = NULL;
static size_t primes_cnt  = 0;
static uint64_t *orders   = NULL;          /* order_{p²}(3) for each prime  */

static void init_primes(void)
{
    if (ready) return;

    size_t limit = LIMIT_PRIME;
    /* allocate a boolean array for composites */
    bool *is_comp = (bool*)calloc(limit + 1, sizeof(bool));
    if (!is_comp) return; /* allocation failure – should not happen */

    /* estimate count of primes to allocate buffer */
    size_t capacity = limit / 10 + 100;
    primes = (uint64_t*)malloc(capacity * sizeof(uint64_t));
    if (!primes) { free(is_comp); return; }

    for (uint64_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (primes_cnt == capacity) {
                capacity <<= 1;
                uint64_t *tmp = (uint64_t*)realloc(primes, capacity * sizeof(uint64_t));
                if (!tmp) break;
                primes = tmp;
            }
            primes[primes_cnt++] = i;
            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);

    /* compute order of 3 modulo p² for every prime */
    orders = (uint64_t*)malloc(primes_cnt * sizeof(uint64_t));
    if (!orders) return;

    for (size_t idx = 0; idx < primes_cnt; ++idx) {
        uint64_t p = primes[idx];
        uint64_t mod = p * p;                     /* p² fits in 64 bits */
        uint64_t phi = p * (p - 1);                /* φ(p²) = p(p‑1) */
        uint64_t ord = phi;
        uint64_t rem = phi;

        /* factor φ(p²) using the same prime list */
        for (size_t j = 0; j < primes_cnt &&
             (uint64_t)primes[j] * (uint64_t)primes[j] <= rem; ++j) {
            uint64_t q = primes[j];
            if (rem % q == 0) {
                while (rem % q == 0) rem /= q;
                while (ord % q == 0 &&
                       mod_pow_u64(3, ord / q, mod) == 1)
                    ord /= q;
            }
        }

        if (rem > 1) { /* remaining prime factor */
            uint64_t q = rem;
            while (ord % q == 0 &&
                   mod_pow_u64(3, ord / q, mod) == 1)
                ord /= q;
        }
        orders[idx] = ord;
    }

    ready = true;
}

/* --- heap element (min‑heap on .next) --------------------------------- */
typedef struct {
    uint64_t prime;   /* p   */
    uint64_t order;   /* ord_{p²}(3) */
    uint64_t next;    /* next k that p can produce */
} Node;

static void heap_push(Node *heap, size_t *size, Node val)
{
    size_t i = (*size)++;
    heap[i] = val;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].next <= heap[i].next) break;
        Node tmp = heap[p]; heap[p] = heap[i]; heap[i] = tmp;
        i = p;
    }
}

static Node heap_pop(Node *heap, size_t *size)
{
    Node ret = heap[0];
    (*size)--;
    if (*size) {
        heap[0] = heap[*size];
        size_t i = 0;
        while (1) {
            size_t l = (i << 1) + 1;
            size_t r = l + 1;
            size_t smallest = i;
            if (l < *size && heap[l].next < heap[smallest].next) smallest = l;
            if (r < *size && heap[r].next < heap[smallest].next) smallest = r;
            if (smallest == i) break;
            Node tmp = heap[i]; heap[i] = heap[smallest]; heap[smallest] = tmp;
            i = smallest;
        }
    }
    return ret;
}

/* --- main sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    init_primes();

    /* build a min‑heap of one node per prime */
    Node *heap = (Node*)malloc(primes_cnt * sizeof(Node));
    if (!heap) return (seq_t)0;  /* allocation failure – return 0 */

    size_t heap_size = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        Node nd;
        nd.prime = primes[i];
        nd.order = orders[i];
        nd.next  = orders[i];   /* first occurrence (k = order) */
        heap_push(heap, &heap_size, nd);
    }

    seq_t result = 0;
    size_t cnt = 0;
    while (cnt < (size_t)n) {
        uint64_t min_next = heap[0].next;
        uint64_t min_prime = UINT64_MAX;

        /* pop all nodes that emit the current minimum k */
        do {
            Node cur = heap_pop(heap, &heap_size);
            if (cur.prime < min_prime) min_prime = cur.prime;
            cur.next += cur.order;
            heap_push(heap, &heap_size, cur);
        } while (heap_size > 0 && heap[0].next == min_next);

        result = (seq_t)min_prime;
        ++cnt;
    }

    free(heap);
    return result;
}
