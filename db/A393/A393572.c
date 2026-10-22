/*
 * A393572  Numbers that are the sum of a square and the reverse of a square.
 * https://oeis.org/A393572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth smallest number that can be written as a square
 *            plus the decimal reverse of another square. a(n) = floor and
 *            count kind-of sequence: numbers = a^2 + rev(b^2)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Reverse decimal digits of a 64‑bit unsigned integer.  The result
   is stored in seq_t and may overflow on small seq_t types, but
   that matches the definition: rev(10000) = 1, etc. */
static inline seq_t reverse_uint64(uint64_t x) {
    seq_t r = 0;
    while (x > 0) {
        r = r * 10 + (x % 10);
        x /= 10;
    }
    return r;
}

/* Size of the table of sequences we merge.  
   The value 4000 is safely large enough to produce tens of thousands
   of terms for typical 64‑bit seq_t builds and keeps the heap small. */
#define B_MAX 4000

static seq_t revs[B_MAX];          /* rev(b^2) for each b          */
static uint32_t a_cnt[B_MAX];      /* current a for each sequence  */
static uint32_t heap_b[B_MAX];     /* b index at each heap position */
static seq_t  heap_sum[B_MAX];     /* current sum at each heap pos */
static bool    init_done = false;  /* flag indicating initialization */

/* Cache of the distinct terms in ascending order. */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_capacity = 0;

/* Initialise the table of sequences and the min‑heap. */
static void init_sequences(void) {
    for (uint32_t b = 0; b < B_MAX; ++b) {
        uint64_t sq = (uint64_t)b * (uint64_t)b;
        revs[b] = reverse_uint64(sq);
        a_cnt[b] = 0;
        heap_b[b] = b;
        heap_sum[b] = revs[b];          /* a = 0  →  rev(b²)          */
    }
    /* Build min‑heap on heap_sum. */
    for (int i = (int)(B_MAX / 2) - 1; i >= 0; --i) {
        size_t idx = (size_t)i;
        while (true) {
            size_t left = 2 * idx + 1;
            size_t right = left + 1;
            size_t smallest = idx;
            if (left < (size_t)B_MAX && heap_sum[left] < heap_sum[smallest])
                smallest = left;
            if (right < (size_t)B_MAX && heap_sum[right] < heap_sum[smallest])
                smallest = right;
            if (smallest == idx)
                break;
            seq_t tmp_sum = heap_sum[idx];
            heap_sum[idx] = heap_sum[smallest];
            heap_sum[smallest] = tmp_sum;
            uint32_t tmp_b = heap_b[idx];
            heap_b[idx] = heap_b[smallest];
            heap_b[smallest] = tmp_b;
            idx = smallest;
        }
    }
    init_done = true;
}

/* Make sure the cache has at least 'needed' capacity. */
static void ensure_cache_capacity(size_t needed) {
    if (cache_capacity < needed) {
        size_t new_cap = cache_capacity ? cache_capacity * 2 : 1024;
        while (new_cap < needed)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* allocation failed */
        cache = tmp;
        cache_capacity = new_cap;
    }
}

/* Generate distinct terms until we have at least 'target' values. */
static void compute_terms_until(idx_t target) {
    if (!init_done)
        init_sequences();

    size_t targ = (size_t)target;
    if (targ <= cache_size)
        return;           /* already enough */

    ensure_cache_capacity(targ);

    while (cache_size < targ) {
        seq_t min_sum = heap_sum[0];
        uint32_t b = heap_b[0];

        /* store unique value */
        if (cache_size == 0 || min_sum != cache[cache_size - 1]) {
            cache[cache_size++] = min_sum;
        }

        /* advance a for this sequence */
        ++a_cnt[b];
        seq_t a_val = (seq_t)a_cnt[b];
        seq_t a_sq  = a_val * a_val;
        heap_sum[0] = revs[b] + a_sq;

        /* heapify down from the root */
        size_t idx = 0;
        while (true) {
            size_t left = 2 * idx + 1;
            size_t right = left + 1;
            size_t smallest = idx;
            if (left < (size_t)B_MAX && heap_sum[left] < heap_sum[smallest])
                smallest = left;
            if (right < (size_t)B_MAX && heap_sum[right] < heap_sum[smallest])
                smallest = right;
            if (smallest == idx)
                break;
            seq_t tmp_sum = heap_sum[idx];
            heap_sum[idx] = heap_sum[smallest];
            heap_sum[smallest] = tmp_sum;
            uint32_t tmp_b = heap_b[idx];
            heap_b[idx] = heap_b[smallest];
            heap_b[smallest] = tmp_b;
            idx = smallest;
        }
    }
}

/* Public API: a(n) */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;          /* safety for offset 1 */
    compute_terms_until(n);
    return cache[(size_t)n - 1];
}
