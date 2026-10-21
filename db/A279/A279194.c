/*
 * A279194  Numbers of the form {(11*h+p)*11^2k-1 | h,k in N and p in
 *          {1,3,4,5,9} } U {(11*h+q)*11^(2k+1)-1 | h,k in N and q in
 *          {2,6,7,8,10} }.
 * https://oeis.org/A279194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers of the form {(11*h+p)*11^(2k) - 1 | h,k∈ℕ,
 *            p∈{1,3,4,5,9}} * ∪ {(11*h+q)*11^(2k+1) - 1 | h,k∈ℕ,
 *            q∈{2,6,7,8,10}}
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- heap helpers ------------------------------------------------- */
static seq_t *heap;
static size_t heap_sz = 0;          /* number of elements in the heap */

/* push value into min‑heap (0‑based) */
static void heap_push(seq_t val)
{
    size_t i = heap_sz++;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent] <= val) break;
        heap[i] = heap[parent];
        i = parent;
    }
    heap[i] = val;
}

/* pop smallest element from the heap */
static seq_t heap_pop(void)
{
    seq_t res = heap[0];
    heap[0] = heap[--heap_sz];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        if (l >= heap_sz) break;
        size_t r = l + 1;
        size_t child = (r < heap_sz && heap[r] < heap[l]) ? r : l;
        if (heap[i] <= heap[child]) break;
        seq_t tmp = heap[i];
        heap[i] = heap[child];
        heap[child] = tmp;
        i = child;
    }
    return res;
}

/* ---------- sequence generation ----------------------------------------- */
static const int P[5] = {1,3,4,5,9};
static const int Q[5] = {2,6,7,8,10};

/* initialise the heap with all k = 0 terms of both families */
static void init_sources(void)
{
    const idx_t HMAX = 5000ULL;        /* large enough for n up to about 10 000 */
    const size_t TOTAL = 10 * (size_t)(HMAX + 1);

    heap = (seq_t*)malloc(TOTAL * sizeof(seq_t));
    heap_sz = 0;

    for (idx_t h = 0; h <= HMAX; ++h) {
        for (int pi = 0; pi < 5; ++pi) {
            seq_t val = ((seq_t)11 * h + P[pi]) - 1;
            heap_push(val);
        }
        for (int qi = 0; qi < 5; ++qi) {
            seq_t base = ((seq_t)11 * h + Q[qi]);
            seq_t val = base * (seq_t)11U - 1;
            heap_push(val);
        }
    }
}

/* ---------- public function ---------------------------------------------- */
seq_t a(idx_t n)
{
    static bool initialised = false;
    static seq_t *seq = NULL;
    static size_t seq_len = 0;          /* number of elements already computed */
    static size_t seq_cap = 0;          /* allocated capacity */

    if (!initialised) {
        initialised = true;
        init_sources();
        seq_cap = 1000;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
    }

    /* extend the sequence until it contains the requested term */
    while (seq_len < n) {
        seq_t val = heap_pop();
        if (seq_len == seq_cap) {
            seq_cap <<= 1;                 /* double capacity */
            seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len++] = val;
        seq_t next = (val + 1) * (seq_t)121U - 1;  /* next in the same geometric progression */
        heap_push(next);
    }

    return seq[n - 1];   /* a(1) is the first term */
}
