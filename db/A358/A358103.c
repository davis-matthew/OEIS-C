/*
 * A358103  Quotient of the n-th divisible pair, where pairs are ordered by
 *          Heinz number. Quotient of prime indices of A318990(n).
 * https://oeis.org/A358103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A358103:  Quotient of the n‑th divisible pair,
    where pairs (i,j) with i | j are ordered by
    prime(i) * prime(j) (Heinz number) and the value is j/i.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------------------------------------------------------------------- */
/*  Prime generation (Sieve of Eratosthenes)                              */
/* ---------------------------------------------------------------------- */
#define PRIME_SIEVE_LIMIT    2000000U   /* enough for 148933 primes */
#define MAX_I                20000U     /* number of initial i candidates */

static uint64_t *g_primes = NULL;    /* array of primes (1‑based) */
static size_t   g_prime_cnt = 0;     /* number of primes stored */

/* build primal array once */
static void build_primes(void)
{
    if (g_primes != NULL) return;           /* already built */

    /* Temporary bit array: 1 = prime, 0 = composite */
    uint8_t *sieve = (uint8_t*)calloc(PRIME_SIEVE_LIMIT + 1, sizeof(uint8_t));
    if (!sieve) abort();

    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i) sieve[i] = 1;
    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_SIEVE_LIMIT; ++i)
        if (sieve[i])
            for (uint64_t j = (uint64_t)i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                sieve[(size_t)j] = 0;

    /* Count primes and allocate array */
    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (sieve[i]) ++g_prime_cnt;
    g_primes = (uint64_t*)malloc(g_prime_cnt * sizeof(uint64_t));
    if (!g_primes) abort();

    /* Store primes */
    size_t pos = 0;
    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (sieve[i]) g_primes[pos++] = (uint64_t)i;

    free(sieve);
}

/* ---------------------------------------------------------------------- */
/*  Minimal heap for generating pairs (i,j)                              */
/* ---------------------------------------------------------------------- */
typedef struct {
    idx_t i;          /* divisor index (1‑based) */
    idx_t j;          /* multiple index (1‑based) */
    seq_t h;          /* Heizen number = prime(i) * prime(j) */
} Node;

static Node *g_heap = NULL;
static size_t g_heap_sz = 0;

static void heap_push(Node node)
{
    size_t idx = g_heap_sz++;
    g_heap[idx] = node;

    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (g_heap[parent].h <= g_heap[idx].h) break;
        Node tmp = g_heap[parent];
        g_heap[parent] = g_heap[idx];
        g_heap[idx] = tmp;
        idx = parent;
    }
}

static void heap_pop(Node *out)
{
    *out = g_heap[0];
    g_heap_sz--;
    g_heap[0] = g_heap[g_heap_sz];

    size_t idx = 0;
    while (true) {
        size_t left  = (idx << 1) + 1;
        size_t right = left + 1;
        size_t smallest = idx;

        if (left  < g_heap_sz && g_heap[left].h  < g_heap[smallest].h)
            smallest = left;
        if (right < g_heap_sz && g_heap[right].h < g_heap[smallest].h)
            smallest = right;
        if (smallest == idx) break;

        Node tmp = g_heap[smallest];
        g_heap[smallest] = g_heap[idx];
        g_heap[idx] = tmp;
        idx = smallest;
    }
}

/* ---------------------------------------------------------------------- */
/*  Cache for computed terms                                               */
/* ---------------------------------------------------------------------- */
static seq_t *g_cache = NULL;
static size_t g_cache_cap = 0;
static size_t g_cache_sz  = 0;

/* ---------------------------------------------------------------------- */
/*  Initialisation of primes, heap and cache                               */
/* ---------------------------------------------------------------------- */
static void init_computations(void)
{
    build_primes();

    if (g_heap) return;   /* already initialised */

    g_heap = (Node*)malloc(MAX_I * sizeof(Node));
    if (!g_heap) abort();

    /* insert first candidate for each i: j = i */
    for (idx_t i = 1; i <= MAX_I && (size_t)i <= g_prime_cnt; ++i) {
        Node nd;
        nd.i = i;
        nd.j = i;
        nd.h = (seq_t)g_primes[(size_t)i-1] * (seq_t)g_primes[(size_t)i-1];
        heap_push(nd);
    }
}

/* ---------------------------------------------------------------------- */
/*  Generate terms until we have at least n elements                       */
/* ---------------------------------------------------------------------- */
static void generate_until(idx_t n)
{
    init_computations();

    if (g_cache == NULL) {
        g_cache_cap = 64;
        g_cache = (seq_t*)malloc(g_cache_cap * sizeof(seq_t));
        if (!g_cache) abort();
    }

    while (g_cache_sz < (size_t)n) {
        Node cur;
        heap_pop(&cur);

        /* store quotient j/i */
        if (g_cache_sz == g_cache_cap) {
            g_cache_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(g_cache, g_cache_cap * sizeof(seq_t));
            if (!tmp) abort();
            g_cache = tmp;
        }
        g_cache[g_cache_sz++] = (seq_t)(cur.j / cur.i);

        /* generate next candidate for same i */
        idx_t next_j = cur.j + cur.i;
        if ((size_t)next_j <= g_prime_cnt) {
            Node nxt;
            nxt.i = cur.i;
            nxt.j = next_j;
            nxt.h = (seq_t)g_primes[(size_t)cur.i-1] * (seq_t)g_primes[(size_t)next_j-1];
            heap_push(nxt);
        }
    }
}

/* ---------------------------------------------------------------------- */
/*  Public function                                                      */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but harmless */

    generate_until(n);
    return g_cache[(size_t)n-1];
}
