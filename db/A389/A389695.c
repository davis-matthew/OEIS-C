/*
 * A389695  Powers k^m, m > 1, where k is neither squarefree, squareful, nor a
 *          product of primorials.
 * https://oeis.org/A389695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 27, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   gen_prod_primorials, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence a(n) is the nth smallest value k^m, m>1, where the base
 *  k has at least one squared prime factor and at least one distinct
 *  prime factor (i.e., it is not squarefree and not squareful) and k
 *  is not a product of primorials.  The algorithm generates the
 *  eligible bases k up to a fixed limit, inserts k^2 into a min‑heap,
 *  and repeatedly extracts the smallest value, generating higher
 *  powers of the same base on demand.  Duplicate values are suppressed.
 *  The function is memoised so that repeated calls to a(n) reuse
 *  previously computed terms.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"
#include "../utils/gen_prod_primorials.h"

typedef struct {
    seq_t val;        /* current power value */
    uint32_t base;    /* base k */
    idx_t   exp;      /* current exponent m  (starts at 2) */
} Node;

typedef struct {
    Node *data;
    size_t size;
    size_t cap;
} Heap;

static void heap_init(Heap *h)
{
    h->size = 0;
    h->cap  = 16;
    h->data = (Node *)malloc(sizeof(Node) * h->cap);
}

static void heap_free(Heap *h)
{
    free(h->data);
    h->data = NULL;
    h->size = h->cap = 0;
}

static void heap_swap(Node *a, Node *b)
{
    Node tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heap_push(Heap *h, Node n)
{
    if (h->size == h->cap) {
        h->cap *= 2;
        h->data = (Node *)realloc(h->data, sizeof(Node) * h->cap);
    }
    size_t i = h->size++;
    h->data[i] = n;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (h->data[p].val <= h->data[i].val) break;
        heap_swap(&h->data[p], &h->data[i]);
        i = p;
    }
}

static Node heap_pop(Heap *h)
{
    Node res = h->data[0];
    h->data[0] = h->data[--h->size];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < h->size && h->data[l].val < h->data[smallest].val)
            smallest = l;
        if (r < h->size && h->data[r].val < h->data[smallest].val)
            smallest = r;
        if (smallest == i) break;
        heap_swap(&h->data[i], &h->data[smallest]);
        i = smallest;
    }
    return res;
}

/*  Hereditary: test whether x has a prime factor with exponent ≥ 2.
 *  Returns true if such a factor exists, false otherwise.  */
static bool not_squarefree(uint32_t x, uint32_t *spf)
{
    while (x > 1) {
        uint32_t p = spf[x];
        int cnt = 0;
        while (x % p == 0) {
            x /= p;
            ++cnt;
            if (cnt > 1) return true;
        }
    }
    return false;  /* squarefree */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n<1 */

    /*  Static tables and state, initialised once.  */
    static bool initialized = false;
    static const uint32_t LIMIT = 200000;                 /* base range limit */
    static uint32_t *spf = NULL;
    static uint32_t primes[200005];
    static size_t   primes_cnt = 0;
    static uint64_t primorials[20];
    static size_t   prim_cnt = 0;
    static uint8_t *is_prod = NULL;

    static Heap heap;
    static seq_t *cache = NULL;
    static size_t  cache_len = 0;
    static size_t  cache_cap = 0;

    if (!initialized) {
        /* 1. smallest‑prime‑factor sieve up to LIMIT.  */
        spf = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
        for (uint32_t i = 0; i <= LIMIT; ++i) spf[i] = 0;
        for (uint32_t i = 2; i <= LIMIT; ++i) {
            if (spf[i] == 0) {
                spf[i] = i;
                primes[primes_cnt++] = i;
                if ((uint64_t)i * i <= LIMIT)
                    for (uint32_t j = i * i; j <= LIMIT; j += i)
                        if (spf[j] == 0) spf[j] = i;
            }
        }

        /* 2. list of primorials (product of first primes).  */
        uint64_t prod = 1;
        for (size_t i = 0; i < primes_cnt; ++i) {
            if (prod > LIMIT / primes[i]) break;
            prod *= primes[i];
            if (prod > LIMIT) break;
            primorials[prim_cnt++] = prod;
        }

        /* 3. mark all products of primorials up to LIMIT.  */
        is_prod = (uint8_t *)calloc(LIMIT + 1, sizeof(uint8_t));
        gen_prod_primorials(0, 1, primorials, prim_cnt, is_prod, LIMIT);

        /* 4. initialise heap.  */
        heap_init(&heap);

        /* 5. push initial powers k^2 for all eligible bases k.  */
        for (uint32_t k = 2; k <= LIMIT; ++k) {
            if (!not_squarefree(k, spf)) continue;       /* must have a square factor */
            /* check existence of a prime exponent equal to 1 (not squareful) */
            uint32_t x = k;
            bool has_exp1 = false;
            while (x > 1) {
                uint32_t p = spf[x];
                int cnt = 0;
                while (x % p == 0) {
                    x /= p;
                    ++cnt;
                }
                if (cnt == 1) has_exp1 = true;
            }
            if (!has_exp1) continue;                     /* would be squareful */
            if (is_prod[k]) continue;                    /* product of primorials */
            seq_t val = seq_mul((seq_t)k, (seq_t)k);
            heap_push(&heap, (Node){val, k, 2});
        }
        initialized = true;
    }

    /*  Memoise computed sequence values.  */
    if (cache_len < (size_t)n) {
        size_t last_val = 0;
        bool  first = true;
        while (cache_len < (size_t)n) {
            Node cur = heap_pop(&heap);
            /* skip duplicates */
            if (first || cur.val != (seq_t)last_val) {
                if (cache_len == cache_cap) {
                    cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = cur.val;
                last_val = (uint64_t)cur.val;
                first = false;
            }
            /* push next power of the same base */
            seq_t next_val = seq_mul(cur.val, (seq_t)cur.base);
            heap_push(&heap, (Node){next_val, cur.base, cur.exp + 1});
        }
    }
    return cache[(size_t)n - 1];
}
