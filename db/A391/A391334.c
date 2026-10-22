/*
 * A391334  Odd semiprimes k = p*q, with p, q primes, such that neither k =
 *          A325820(p,x) nor k = A325820(q,x) for any x, where A325820 is the
 *          carryless base-3 multiplication.
 * https://oeis.org/A391334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 2619, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_cmp, seq_to_base3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS Sequence A391334
    a(n) : odd semiprime k = p*q (p,q odd primes) such that
           k ≠ A325820(p,x) and k ≠ A325820(q,x) for all x.
    Computation: generate odd semiprimes in ascending order and
    test divisibility of the base‑3 carry‑less representation.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_cmp.h"
#include "../utils/seq_to_base3.h"

typedef struct {
    uint8_t len;            /* number of base‑3 digits */
    uint8_t digits[20];     /* least‑significant first */
} PrimeInfo;

/* ----- static data (initialised once) ----- */
static bool  _init_done = false;

static uint32_t          * _odd_primes   = NULL;
static PrimeInfo         * _prime_facts  = NULL;
static size_t               _prime_cnt    = 0;   /* number of odd primes */

static struct HeapNode {
    seq_t   val;
    size_t  i, j;                /* indices in _odd_primes */
} * _heap         = NULL;
static size_t  _heap_sz = 0;     /* number of nodes in heap */

/* cache of sequence values */
static seq_t * _cache   = NULL;
static size_t  _cache_sz = 0;
static size_t  _cache_cap = 0;

/* helper macro for seq_t comparison (handles arbitrary types) */
static inline int seq_lt(seq_t a, seq_t b) { return seq_cmp(&a, &b) < 0; }

/* ---------- heap operations (1‑based) ---------- */
static inline void heap_swap(size_t a, size_t b)
{
    struct HeapNode tmp = _heap[a];
    _heap[a] = _heap[b];
    _heap[b] = tmp;
}
static void heap_down(size_t pos)
{
    while (1) {
        size_t left  = pos*2;
        if (left > _heap_sz) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right <= _heap_sz && seq_lt(_heap[right].val, _heap[left].val))
            smallest = right;
        if (seq_lt(_heap[smallest].val, _heap[pos].val)) {
            heap_swap(pos, smallest);
            pos = smallest;
        } else break;
    }
}
static void heap_push(struct HeapNode node)
{
    ++_heap_sz;
    _heap[_heap_sz] = node;
    size_t pos = _heap_sz;
    while (pos > 1) {
        size_t parent = pos / 2;
        if (seq_lt(_heap[pos].val, _heap[parent].val)) {
            heap_swap(pos, parent);
            pos = parent;
        } else break;
    }
}
static struct HeapNode heap_pop(void)
{
    struct HeapNode top = _heap[1];
    _heap[1] = _heap[_heap_sz];
    --_heap_sz;
    heap_down(1);
    return top;
}

/* ---------- divisibility test: k % P(p) == 0 ? ---------- */
static bool div_by_prime(seq_t k, const PrimeInfo *pinfo)
{
    uint8_t kin[64];  /* enough for k up to ~10^15 */
    int kl = 0;
    seq_to_base3(k, kin, &kl);
    if ((size_t)kl < pinfo->len) return false;          /* degree too small */

    uint8_t inv = pinfo->digits[pinfo->len-1];           /* leading coeff (1 or 2) */
    for (int idx = kl - 1; idx >= (int)pinfo->len - 1; --idx) {
        uint8_t coeff = kin[idx];
        if (coeff == 0) continue;
        uint8_t factor = (coeff * inv) % 3;             /* coeff/leading in GF(3) */
        int shift = idx - ((int)pinfo->len - 1);
        for (int d = 0; d < (int)pinfo->len; ++d) {
            uint8_t sub = (factor * pinfo->digits[d]) % 3;
            uint8_t val = kin[shift + d] + 3 - sub;
            if (val >= 3) val -= 3;
            kin[shift + d] = val;
        }
    }
    for (int t = 0; t < (int)pinfo->len - 1; ++t)
        if (kin[t] != 0) return false;
    return true;
}

/* ---------- prime sieve and initial data construction ---------- */
static void init(void)
{
    if (_init_done) return;

    const uint32_t MAX_PRIME = 2000000;            /* enough for n≈10000 */
    uint8_t *is_comp = (uint8_t*)calloc(MAX_PRIME + 1, 1);

    /* first pass: collect all primes */
    size_t cap = 200000;
    size_t cnt = 0;
    uint32_t *primes = (uint32_t*)malloc(cap * sizeof(uint32_t));
    for (uint32_t i = 2; i <= MAX_PRIME; ++i) {
        if (!is_comp[i]) {
            if (cnt == cap) { cap *= 2; primes = (uint32_t*)realloc(primes, cap * sizeof(uint32_t)); }
            primes[cnt++] = i;
            if ((uint64_t)i * i <= MAX_PRIME) {
                for (uint64_t j = (uint64_t)i * i; j <= MAX_PRIME; j += i)
                    is_comp[(size_t)j] = 1;
            }
        }
    }

    /* second pass: keep only odd primes (exclude 2) */
    _prime_cnt = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] >= 3 && (primes[i] & 1)) ++_prime_cnt;
    }
    _odd_primes = (uint32_t*)malloc(_prime_cnt * sizeof(uint32_t));
    for (size_t i = 0, j = 0; i < cnt; ++i) {
        if (primes[i] >= 3 && (primes[i] & 1))
            _odd_primes[j++] = primes[i];
    }
    free(primes);

    /* prime factor digits */
    _prime_facts = (PrimeInfo*)malloc(_prime_cnt * sizeof(PrimeInfo));
    for (size_t i = 0; i < _prime_cnt; ++i) {
        uint32_t v = _odd_primes[i];
        uint8_t len = 0;
        while (v > 0) {
            _prime_facts[i].digits[len++] = v % 3;
            v /= 3;
        }
        _prime_facts[i].len = len;
    }

    /* initialise heap with pairs (i,i) */
    _heap = (struct HeapNode*)malloc((_prime_cnt + 1) * sizeof(struct HeapNode));
    _heap_sz = _prime_cnt;
    for (size_t i = 0; i < _prime_cnt; ++i) {
        _heap[i+1].i = i;
        _heap[i+1].j = i;
        _heap[i+1].val = (seq_t)_odd_primes[i] * (seq_t)_odd_primes[i];
    }
    for (size_t h = _heap_sz / 2; h >= 1; --h) heap_down(h);

    _init_done = true;
    free(is_comp);
}

/* ---------- sequence value generator ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for n<1, but safe */

    init();

    if ((size_t)n > _cache_cap) {
        size_t new_cap = _cache_cap ? _cache_cap * 2 : 128;
        if (new_cap < (size_t)n) new_cap = n;
        _cache = (seq_t*)realloc(_cache, new_cap * sizeof(seq_t));
        _cache_cap = new_cap;
    }

    while (_cache_sz < (size_t)n) {
        struct HeapNode node = heap_pop();
        seq_t k = node.val;

        bool valid = true;
        if (div_by_prime(k, &_prime_facts[node.i]) ||
            div_by_prime(k, &_prime_facts[node.j]))
            valid = false;

        if (valid) {
            _cache[_cache_sz++] = k;
        }

        /* push the next pair for the same i */
        if (node.j + 1 < _prime_cnt) {
            struct HeapNode next;
            next.i = node.i;
            next.j = node.j + 1;
            next.val = (seq_t)_odd_primes[node.i] * (seq_t)_odd_primes[next.j];
            heap_push(next);
        }
    }

    return _cache[n-1];
}
