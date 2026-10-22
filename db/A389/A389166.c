/*
 * A389166  a(n) is the largest square dividing the numbers satisfying Euler's
 *          condition for odd perfect numbers (A228058).
 * https://oeis.org/A389166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) – the largest square divisor of the nth integer that
 *            satisfies Euler’s condition for an odd perfect number (A228058).
 *            Each such integer is of the form p^(1+4k)·r² with p≡1 (mod 4),
 *            k≥0, r>1 odd and gcd(p,r)=1. For that number n, a(n)=p^(4k)·r²,
 *            which equals n/p
 * Range:     u8 none, u16 n <= 3, u32 n <= 8810, u64 n <= 8810,
 *            u128 n <= 8810, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation generates the sequence in strictly increasing
    order of the underlying numbers n by merging all arithmetic
    progressions p^(1+4k)·r² for all special primes p≡1 (mod 4).
    It maintains a min‑heap of the next candidate from each
    (p,k,r) triple, and produces the required large‑square divisor
    on each extraction.  The sequence is memoised for arbitrary
    call order.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/big_pow.h"

/* ----------  Global data for primes and heap  ---------- */
static bool primes_inited = false;
static uint64_t *specialPrimes = NULL;   /* primes ≡1 mod4 */
static size_t specialPrimeCnt = 0;

/* Simple sieve to collect all primes ≡1 mod4 up to a fixed bound */
static void init_special_primes(void)
{
    const size_t LIMIT = 400000;                 /* enough for first ~10‑k terms   */
    bool *isPrime = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    memset(isPrime, true, (LIMIT + 1) * sizeof(bool));
    isPrime[0] = isPrime[1] = false;
    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (isPrime[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                isPrime[j] = false;

    /* first pass: count required space */
    size_t cnt = 0;
    for (size_t i = 5; i <= LIMIT; i += 4)          /* 5,9,13,... */
        if (isPrime[i]) ++cnt;

    specialPrimes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 5; i <= LIMIT; i += 4)
        if (isPrime[i]) specialPrimes[idx++] = (uint64_t)i;

    free(isPrime);
    specialPrimeCnt = cnt;
    primes_inited = true;
}

/* ----------  Heap node definition  ---------- */
typedef struct {
    uint64_t p;      /* special prime */
    uint64_t r;      /* current odd r  (r>1, coprime to p) */
    seq_t   powp;    /* p^(4k)   (k is implicit from powp)      */
    seq_t   n;       /* next candidate value = p * powp * r^2   */
} node_t;

/* ----------  Min‑heap operations  ---------- */
static node_t *heap = NULL;
static size_t heap_size = 0, heap_cap = 0;
static bool heap_inited = false;

static inline void sift_up(size_t pos)
{
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (heap[parent].n <= heap[pos].n) break;
        node_t tmp = heap[parent]; heap[parent] = heap[pos]; heap[pos] = tmp;
        pos = parent;
    }
}

static inline void sift_down(size_t pos)
{
    while (true) {
        size_t left  = (pos << 1) | 1;
        size_t right = left + 1;
        size_t smallest = pos;
        if (left < heap_size && heap[left].n < heap[smallest].n) smallest = left;
        if (right < heap_size && heap[right].n < heap[smallest].n) smallest = right;
        if (smallest == pos) break;
        node_t tmp = heap[smallest]; heap[smallest] = heap[pos]; heap[pos] = tmp;
        pos = smallest;
    }
}

static void heap_push(node_t node)
{
    if (heap_size == heap_cap) {
        heap_cap = heap_cap ? heap_cap * 2 : 8;
        heap = (node_t*)realloc(heap, heap_cap * sizeof(node_t));
    }
    heap[heap_size++] = node;
    sift_up(heap_size - 1);
}

static node_t heap_pop(void)
{
    node_t res = heap[0];
    heap[0] = heap[--heap_size];
    sift_down(0);
    return res;
}

/* ----------  Sequence storage  ---------- */
static seq_t *seq_array = NULL;
static size_t seq_len = 0, seq_cap = 0;

static inline void ensure_seq_capacity(void)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 8;
        seq_array = (seq_t*)realloc(seq_array, seq_cap * sizeof(seq_t));
    }
}

/* ----------  Public function  ---------- */
seq_t a(idx_t n)
{
    /* initialise primes and heap once */
    if (!primes_inited) init_special_primes();
    if (!heap_inited) {
        heap_cap   = specialPrimeCnt;
        heap       = (node_t*)malloc(heap_cap * sizeof(node_t));
        heap_size  = 0;
        for (size_t i = 0; i < specialPrimeCnt; ++i) {
            uint64_t p = specialPrimes[i];
            seq_t n_val = (seq_t)p * (seq_t)9;          /* p * 3² */
            node_t nd = {p, 3, (seq_t)1, n_val};
            heap_push(nd);
        }
        heap_inited = true;
    }
    /* initialise sequence array once */
    if (!seq_array) {
        seq_cap = 8;
        seq_array = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq_len  = 0;
    }

    /* generate terms up to requested n */
    while (seq_len < (size_t)n) {
        node_t cur = heap_pop();

        /* a(n) = n / p = powp * r² */
        seq_t largest_square = cur.n / (seq_t)cur.p;
        ensure_seq_capacity();
        seq_array[seq_len++] = largest_square;

        /* push next value with the same k (increment r) */
        uint64_t r_next = cur.r + 2;
        while (r_next % cur.p == 0) r_next += 2;    /* maintain gcd(p,r)=1 */
        seq_t new_n = (seq_t)cur.p * cur.powp *
                      ((seq_t)r_next * (seq_t)r_next);
        node_t nd_next = {cur.p, r_next, cur.powp, new_n};
        heap_push(nd_next);

        /* if this was the first r (r==3), seed next k */
        if (cur.r == 3) {
            seq_t powp_next = cur.powp * big_pow((seq_t)cur.p, (seq_t)4);
            seq_t n_next = (seq_t)cur.p * powp_next *
                           ((seq_t)3 * (seq_t)3);
            node_t nd_knext = {cur.p, 3, powp_next, n_next};
            heap_push(nd_knext);
        }
    }

    return seq_array[n - 1];
}
