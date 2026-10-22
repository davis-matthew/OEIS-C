/*
 * A365745  Powers s^m, m > 1, where s is a composite squarefree number that
 *          is not a primorial.
 * https://oeis.org/A365745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 12, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   build_spf, cmp_seq_t, is_prime, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A365745: powers s^m, m > 1, where s is a composite square‑free number
 * that is not a primorial.  The function returns a(n) for n ≥ 1.  The terms are
 * generated lazily and cached so that any order of calls is supported. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"
#include "../utils/is_prime.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/cmp_seq_t.h"

typedef struct {
    seq_t val;        /* current power value */
    idx_t base;       /* base s (squarefree composite) */
    int exp;          /* current exponent (≥2) */
} Node;

typedef struct {
    Node *data;
    size_t size;
    size_t cap;
} Heap;

/*--------------------- Heap helpers -------------------------------------*/
static inline int node_less(const Node *a, const Node *b)
{
    return cmp_seq_t(&a->val, &b->val) < 0;
}
static inline void heap_swap(Node *a, Node *b)
{
    Node tmp = *a; *a = *b; *b = tmp;
}
static void heap_expand(Heap *h)
{
    size_t nc = h->cap * 2 + 1;
    Node *nd = (Node*)realloc(h->data, nc * sizeof(Node));
    if (!nd) abort();
    h->data = nd; h->cap = nc;
}
static void heap_init(Heap *h, size_t cap)
{
    h->data = (Node*)malloc(cap * sizeof(Node));
    if (!h->data) abort();
    h->size = 0; h->cap = cap;
}
static void heap_push(Heap *h, Node n)
{
    if (h->size == h->cap) heap_expand(h);
    size_t i = h->size++;
    h->data[i] = n;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (!node_less(&h->data[i], &h->data[p])) break;
        heap_swap(&h->data[i], &h->data[p]);
        i = p;
    }
}
static Node heap_pop(Heap *h)
{
    Node top = h->data[0];
    h->data[0] = h->data[--h->size];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < h->size && node_less(&h->data[l], &h->data[smallest])) smallest = l;
        if (r < h->size && node_less(&h->data[r], &h->data[smallest])) smallest = r;
        if (smallest == i) break;
        heap_swap(&h->data[i], &h->data[smallest]);
        i = smallest;
    }
    return top;
}

/*--------------------- Global state -------------------------------------*/
static bool initialized = false;
static Heap global_heap;
static seq_t *cache = NULL;
static size_t cache_size = 0, cache_cap = 0;
static seq_t last_val;
static bool has_last = false;

/*--------------------- Sequence initialization --------------------------*/
static void initialize_sequence(void)
{
    if (initialized) return;

    /* Choose a limit that comfortably contains enough bases for at least 10 000 terms. */
    const idx_t MAX_BASE = 4800000U;   /* 4.8 million is adequate for 10 000 terms */

    /* 1. Smallest prime factor table. */
    idx_t *spf = NULL;
    build_spf(MAX_BASE, &spf);

    /* 2. List of primes (needed to detect primorials). */
    uint64_t *primes = NULL;
    size_t pcap = 64, pcnt = 0;
    primes = (uint64_t*)malloc(pcap * sizeof(uint64_t));
    if (!primes) abort();
    for (idx_t i = 2; i <= MAX_BASE; ++i) {
        if (spf[i]==i) { /* i is prime */
            if (pcnt==pcap) {
                pcap *= 2;
                uint64_t *tmp = (uint64_t*)realloc(primes, pcap * sizeof(uint64_t));
                if (!tmp) abort();
                primes = tmp;
            }
            primes[pcnt++] = i;
        }
    }

    /* 3. Primorials (products of the first k primes). */
    uint64_t prim_prod = 1;
    uint64_t *primorials = NULL;
    size_t primcap = 16, primcnt = 0;
    primorials = (uint64_t*)malloc(primcap * sizeof(uint64_t));
    if (!primorials) abort();
    for (size_t i = 0; i < pcnt; ++i) {
        if (prim_prod > MAX_BASE / primes[i]) break; /* avoid overflow */
        prim_prod *= primes[i];
        if (primcnt==primcap) {
            primcap *= 2;
            uint64_t *tmp = (uint64_t*)realloc(primorials, primcap * sizeof(uint64_t));
            if (!tmp) abort();
            primorials = tmp;
        }
        primorials[primcnt++] = prim_prod;
    }

    /* 4. Find all composite square‑free bases that are NOT primorials. */
    idx_t *bases = NULL;
    size_t bcap = 64, bcnt = 0;
    bases = (idx_t*)malloc(bcap * sizeof(idx_t));
    if (!bases) abort();
    for (idx_t s = 4; s <= MAX_BASE; ++s) {
        if (is_prime(s)) continue;   /* s must be composite */
        /* factorize using spf to test square‑freeness and count distinct primes */
        idx_t x = s, lastp = 0;
        size_t distinct = 0;
        bool squarefree = true;
        while (x > 1) {
            idx_t p = spf[x];
            if (p == lastp) { squarefree = false; break; }
            ++distinct; lastp = p;
            x /= p;
        }
        if (!squarefree || distinct < 2) continue; /* not square‑free or not composite */

        /* check if s is a primorial */
        bool is_primorial = false;
        for (size_t i = 0; i < primcnt; ++i) {
            if (primorials[i] == (uint64_t)s) { is_primorial = true; break; }
        }
        if (is_primorial) continue;

        if (bcnt==bcap) {
            bcap *= 2;
            idx_t *tmp = (idx_t*)realloc(bases, bcap * sizeof(idx_t));
            if (!tmp) abort();
            bases = tmp;
        }
        bases[bcnt++] = s;
    }

    /* 5. Initialize heap with first power (exponent 2) of every base. */
    heap_init(&global_heap, bcnt);
    for (size_t i = 0; i < bcnt; ++i) {
        seq_t first = seq_pow((seq_t)bases[i], (idx_t)2);
        Node nd = { first, bases[i], 2 };
        heap_push(&global_heap, nd);
    }

    /* Free auxiliary tables; heap remains allocated. */
    free(primes);
    free(primorials);
    free(bases);
    free(spf);

    initialized = true;
}

/*--------------------- Sequence function ---------------------------------*/
seq_t a(idx_t n)
{
    if (!initialized) initialize_sequence();
    if (n == 0) return 0;   /* undefined, but harmless */

    /* Ensure cache has enough capacity for n terms. */
    if (n > cache_cap) {
        size_t newcap = cache_cap ? cache_cap * 2 : 64;
        if (n > newcap) newcap = n;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp; cache_cap = newcap;
    }

    /* Generate terms until we reach the requested index. */
    while (cache_size < (size_t)n) {
        Node cur = heap_pop(&global_heap);

        /* deduplicate consecutive equal values */
        if (!has_last || cmp_seq_t(&cur.val, &last_val) != 0) {
            cache[cache_size++] = cur.val;
            last_val = cur.val; has_last = true;
        }

        /* push next power of this base */
        cur.exp++;
        seq_t next = seq_mul(cur.val, (seq_t)cur.base);
        cur.val = next;
        heap_push(&global_heap, cur);
    }

    return cache[n-1];
}
