/*
 * A261690  a(1) = 1; for n>1, a(n) is the smallest number not already present
 *          which is entailed by the rules (i) k present => 3*k+1 present;
 *          (ii) 2*k present => k present.
 * https://oeis.org/A261690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 7022, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   heap_swap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A261690 – greedy construction for the OEIS sequence.
   a(1) = 1; for n > 1, a(n) is the smallest positive integer not yet
   present in the sequence that must be present by the rules:  
   (i) if k is present then 3*k + 1 must be present;  
   (ii) if 2*k is present then k must be present.
   This implementation uses static arrays of fixed size (large enough for
   n ≤ 10 000) to avoid dynamic allocation problems with arbitrary‑precision
   seq_t types.  The algorithm is O(n log n).  The code is valid C11 and
   C++17 but does not use any C++ specific features. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/heap_swap.h"

/* comparison for seq_t (works for integral types and big‑integer classes) */
static inline int seq_cmp(const seq_t *a, const seq_t *b)
{
    return (*a < *b) ? -1 : (*a == *b ? 0 : 1);
}

/* binary search: true if x is in sorted arr[0..len-1] */
static bool seq_present(const seq_t *arr, size_t len, seq_t x)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        int c = seq_cmp(&arr[mid], &x);
        if (c == 0) return true;
        if (c < 0)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

/* index where x should be inserted into sorted arr[0..len-1] */
static size_t insert_pos(const seq_t *arr, size_t len, seq_t x)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (seq_cmp(&arr[mid], &x) < 0)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* min‑heap utilities ----------------------------------------------- */

static seq_t heap[400000];
static size_t heap_len = 0;

static void heap_push(seq_t value)
{
    size_t i = heap_len++;
    heap[i] = value;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (seq_cmp(&heap[p], &heap[i]) <= 0)
            break;
        heap_swap(&heap[p], &heap[i]);
        i = p;
    }
}

static seq_t heap_pop_min(void)
{
    seq_t min = heap[0];
    seq_t last = heap[--heap_len];
    size_t i = 0;
    while (1) {
        size_t left = (i << 1) + 1;
        size_t right = left + 1;
        if (left >= heap_len)
            break;
        size_t child = (right < heap_len &&
                        seq_cmp(&heap[right], &heap[left]) < 0) ?
                       right : left;
        if (seq_cmp(&heap[child], &last) >= 0)
            break;
        heap[i] = heap[child];
        i = child;
    }
    heap[i] = last;
    return min;
}

/* sequence storage ----------------------------------------------- */

static seq_t seq[200000];
static size_t seq_len = 0;

static seq_t keys[200000];
static size_t keys_len = 0;

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined case */

    /* initialize on first call */
    if (seq_len == 0) {
        seq[0] = (seq_t)1;
        seq_len = 1;
        keys[0] = (seq_t)1;
        keys_len = 1;

        /* rule (i) for k = 1 gives 3*1+1 = 4 */
        heap_push((seq_t)4);
    }

    while (seq_len < n) {
        seq_t x;
        /* find next forced number that is not already present */
        do {
            /* the heap must never be empty here; if it is, the algorithm
               has reached a dead end (theoretical impossibility). */
            x = heap_pop_min();
        } while (seq_present(keys, keys_len, x));

        /* append to sequence */
        seq[seq_len++] = x;

        /* insert x into sorted keys array */
        size_t pos = insert_pos(keys, keys_len, x);
        for (size_t i = keys_len; i > pos; --i)
            keys[i] = keys[i - 1];
        keys[pos] = x;
        keys_len++;

        /* apply the two rules for the new element x */
        /* rule (ii) – if x is even, its half must be present */
        if ((x % (seq_t)2) == (seq_t)0)
            heap_push(x / (seq_t)2);

        /* rule (i) – enforce 3*x + 1 */
        heap_push(x * (seq_t)3 + (seq_t)1);
    }

    return seq[n - 1];
}
