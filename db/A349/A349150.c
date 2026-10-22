/*
 * A349150  Heinz numbers of integer partitions with at most one odd part.
 * https://oeis.org/A349150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   union of Heinz numbers with all even prime indices (A066207) and
 *            those with exactly one odd prime index (A349158)
 * Range:     u8 n <= 1, u16 n <= 19, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- global data ---------- */
static bool *prime_flags   = NULL;
static uint32_t *prime_list = NULL;
static size_t prime_count   = 0;

static uint32_t *even_primes = NULL;
static size_t even_prime_cnt = 0;
static uint32_t *odd_primes  = NULL;
static size_t odd_prime_cnt  = 0;

/* ---------- even‑index‑prime products (E) ---------- */
static seq_t *even_seq = NULL;          /* E[0] = 1, then products of even primes */
static size_t even_len = 0, even_cap = 0;
static size_t *even_ptrs = NULL;        /* index in E used for each even prime */
static seq_t *even_cand = NULL;         /* current candidate for each even prime */

/* ---------- union of E and odd×E (U) ---------- */
static seq_t *union_seq = NULL;
static size_t union_len = 0, union_cap = 0;
static size_t even_merge_idx = 0;       /* index of next unused element in E for the even part */

/* ---------- heap for odd primes × E ---------- */
struct HeapNode {
    seq_t val;
    size_t odd_idx;            /* index into odd_primes array */
};
static struct HeapNode *heap = NULL;
static size_t heap_n = 0;
static size_t *odd_ptrs = NULL;

/* ---------- initialization flag ---------- */
static bool initialized = false;

/* ------------------------------------------------- helpers ------------------------------------------------- */

static void ensure_even_cap(size_t min_cap) {
    if (even_cap >= min_cap) return;
    size_t new_cap = even_cap ? even_cap : 4;
    while (new_cap < min_cap) new_cap <<= 1;
    even_cap = new_cap;
    even_seq = (seq_t*)realloc(even_seq, even_cap * sizeof(seq_t));
}

static void ensure_union_cap(size_t min_cap) {
    if (union_cap >= min_cap) return;
    size_t new_cap = union_cap ? union_cap : 4;
    while (new_cap < min_cap) new_cap <<= 1;
    union_cap = new_cap;
    union_seq = (seq_t*)realloc(union_seq, union_cap * sizeof(seq_t));
}

static void heap_swap(size_t i, size_t j) {
    struct HeapNode tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
}

static void heap_push(seq_t val, size_t odd_idx) {
    heap[heap_n].val = val;
    heap[heap_n].odd_idx = odd_idx;
    size_t i = heap_n++;
    while (i) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= heap[i].val) break;
        heap_swap(p, i);
        i = p;
    }
}

static void heap_pop(seq_t *out_val, size_t *out_odd_idx) {
    *out_val = heap[0].val;
    *out_odd_idx = heap[0].odd_idx;
    heap[0] = heap[--heap_n];
    size_t i = 0;
    for (;;) {
        size_t l = (i << 1) + 1, r = l + 1;
        size_t smallest = i;
        if (l < heap_n && heap[l].val < heap[smallest].val) smallest = l;
        if (r < heap_n && heap[r].val < heap[smallest].val) smallest = r;
        if (smallest == i) break;
        heap_swap(i, smallest);
        i = smallest;
    }
}

/* --------------------------------------------------- prime sieve ---------------------------------------------- */
static void build_primes(size_t limit) {
    if (prime_flags) free(prime_flags);
    prime_flags = (bool*)calloc(limit + 1, sizeof(bool));
    for (size_t i = 2; i * i <= limit; ++i)
        if (!prime_flags[i])
            for (size_t j = i * i; j <= limit; j += i)
                prime_flags[j] = true;

    prime_count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!prime_flags[i]) ++prime_count;

    prime_list = (uint32_t*)realloc(prime_list, prime_count * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!prime_flags[i]) prime_list[idx++] = (uint32_t)i;

    odd_prime_cnt  = (prime_count + 1) >> 1;
    even_prime_cnt = prime_count >> 1;

    odd_primes   = (uint32_t*)realloc(odd_primes,   odd_prime_cnt *  sizeof(uint32_t));
    even_primes  = (uint32_t*)realloc(even_primes,  even_prime_cnt*sizeof(uint32_t));

    idx = 0;
    for (size_t i = 0; i < prime_count; ++i)
        if ((i & 1) == 0) odd_primes[idx++] = prime_list[i];
    idx = 0;
    for (size_t i = 0; i < prime_count; ++i)
        if ((i & 1) == 1) even_primes[idx++] = prime_list[i];
}

/* ------------------------------------------------ initialization ------------------------------------------------ */

static void sequence_init(void) {
    if (initialized) return;

    /* Enough primes for n up to about 10 000 */
    build_primes(200000);

    /* Even products E */
    even_cap = 4;
    even_seq = (seq_t*)malloc(even_cap * sizeof(seq_t));
    even_seq[0] = (seq_t)1;
    even_len  = 1;

    even_ptrs = (size_t*)calloc(even_prime_cnt, sizeof(size_t));
    even_cand = (seq_t*)calloc(even_prime_cnt, sizeof(seq_t));
    for (size_t i = 0; i < even_prime_cnt; ++i)
        even_cand[i] = (seq_t)even_primes[i] * even_seq[0];

    /* Odd primes */
    odd_ptrs = (size_t*)calloc(odd_prime_cnt, sizeof(size_t));

    /* Heap for odd×E */
    heap = (struct HeapNode*)malloc(odd_prime_cnt * sizeof(struct HeapNode));
    heap_n = 0;
    for (size_t i = 0; i < odd_prime_cnt; ++i)
        heap_push((seq_t)odd_primes[i] * even_seq[0], i);

    /* Union sequence */
    union_cap = 4;
    union_seq = (seq_t*)malloc(union_cap * sizeof(seq_t));
    union_len = 0;
    even_merge_idx = 0;

    initialized = true;
}

/* ------------------------------------------------- ensure even seq ------------------------------------------- */

static void ensure_even_len(size_t need) {
    /* need is a 0‑based index that E must support */
    if (even_len > need) return;
    while (even_len <= need) {
        /* minimum candidate among even primes */
        seq_t min_val = even_cand[0];
        for (size_t i = 1; i < even_prime_cnt; ++i)
            if (even_cand[i] < min_val) min_val = even_cand[i];

        /* add to E */
        ensure_even_cap(even_len + 1);
        even_seq[even_len++] = min_val;

        /* advance primes that produced this value */
        for (size_t i = 0; i < even_prime_cnt; ++i) {
            if (even_cand[i] == min_val) {
                ++even_ptrs[i];
                ensure_even_len(even_ptrs[i]);          /* make sure even_seq has the required index */
                even_cand[i] = (seq_t)even_primes[i] * even_seq[even_ptrs[i]];
            }
        }
    }
}

/* ------------------------------------------------- ensure union seq ------------------------------------------ */

static void ensure_union_len(idx_t n) {
    if (union_len >= (size_t)n) return;
    while (union_len < (size_t)n) {
        /* make sure the next even value is available */
        ensure_even_len(even_merge_idx);
        seq_t ev_val = even_seq[even_merge_idx];
        seq_t heap_val = 0;
        bool have_heap = heap_n > 0;
        if (have_heap) heap_val = heap[0].val;

        if (!have_heap || ev_val <= heap_val) {
            /* take even value */
            if (union_len == 0 || ev_val != union_seq[union_len - 1]) {
                ensure_union_cap(union_len + 1);
                union_seq[union_len++] = ev_val;
            }
            ++even_merge_idx;
        } else {
            /* take from heap (odd×E) */
            seq_t val; size_t odd_idx;
            heap_pop(&val, &odd_idx);
            if (union_len == 0 || val != union_seq[union_len - 1]) {
                ensure_union_cap(union_len + 1);
                union_seq[union_len++] = val;
            }
            /* advance odd pointer */
            ++odd_ptrs[odd_idx];
            ensure_even_len(odd_ptrs[odd_idx]);
            seq_t new_val = (seq_t)odd_primes[odd_idx] * even_seq[odd_ptrs[odd_idx]];
            heap_push(new_val, odd_idx);
        }
    }
}

/* --------------------------------------------------- public --------------------------------------------------- */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;      /* undefined, but safe */
    sequence_init();
    ensure_union_len(n);
    return union_seq[n - 1];
}
