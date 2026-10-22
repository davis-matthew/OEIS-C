/*
 * A364974  a(n) is the least positive number that can be written in exactly n
 *          ways as 3*x*y^2 - x^3 where x and y are positive integers.
 * https://oeis.org/A364974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   compute_v_checked
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A364974: a(n) is the least positive number that can be
   written in exactly n ways as 3*x*y*y - x*x*x, with x and y positive integers.
   The function generates values in increasing order using a min‑heap and
   counts their occurrences. No tables or hash tables are used.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/compute_v_checked.h"

/* ---------- integer square root for uint64_t ---------- */
static uint32_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = ((uint64_t)1 << 32);
    while (lo < hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * (__uint128_t)mid;
        if (sq <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo * lo > n)
        lo--;
    return (uint32_t)lo;
}

/* ---------- smallest y such that 3*y*y > x*x ---------- */
static uint32_t min_y_for_x(uint32_t x)
{
    uint64_t xx = (uint64_t)x * (uint64_t)x;   /* x^2  */
    uint64_t t  = xx / 3;                      /* floor(x^2/3) */
    uint32_t s  = isqrt_u64(t);                /* floor(sqrt(t)) */
    return s + 1;                              /* minimal y */
}

/* ---------- heap node ---------- */
typedef struct {
    seq_t v;
    uint32_t x;
    uint32_t y;
} HeapNode;

/* ---------- global state ---------- */
static bool          initialized = false;
static seq_t        *min_values  = NULL;    /* min value for each count */
static size_t        min_cap     = 0;
static idx_t         max_needed  = 0;

static HeapNode      *heap_data   = NULL;
static size_t        heap_size   = 0;
static size_t        heap_cap    = 0;

static uint32_t      next_x_to_add = 1;      /* next x to introduce into heap */

/* ---------- heap utilities ---------- */
static void heap_swap(HeapNode *a, HeapNode *b)
{
    HeapNode tmp = *a; *a = *b; *b = tmp;
}

static void heap_sift_up(size_t pos)
{
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (heap_data[parent].v <= heap_data[pos].v) break;
        heap_swap(&heap_data[parent], &heap_data[pos]);
        pos = parent;
    }
}

static void heap_sift_down(size_t pos)
{
    while (1) {
        size_t left  = (pos << 1) | 1;
        size_t right = left + 1;
        size_t smallest = pos;
        if (left < heap_size && heap_data[left].v < heap_data[smallest].v)
            smallest = left;
        if (right < heap_size && heap_data[right].v < heap_data[smallest].v)
            smallest = right;
        if (smallest == pos) break;
        heap_swap(&heap_data[pos], &heap_data[smallest]);
        pos = smallest;
    }
}

static void heap_push(HeapNode node)
{
    if (heap_size == heap_cap) {
        size_t new_cap = heap_cap * 2 + 1;
        HeapNode *new_heap = (HeapNode*)realloc(heap_data, new_cap * sizeof(HeapNode));
        if (!new_heap) abort();
        heap_data = new_heap;
        heap_cap  = new_cap;
    }
    heap_data[heap_size++] = node;
    heap_sift_up(heap_size - 1);
}

static HeapNode heap_pop(void)
{
    HeapNode top = heap_data[0];
    heap_data[0] = heap_data[--heap_size];
    heap_sift_down(0);
    return top;
}

/* ---------- grow min_values ---------- */
static void grow_min_values(size_t new_cap)
{
    seq_t *new_arr = (seq_t*)calloc(new_cap, sizeof(seq_t));
    if (!new_arr) abort();
    if (min_values) {
        memcpy(new_arr, min_values, min_cap * sizeof(seq_t));
        free(min_values);
    }
    min_values = new_arr;
    min_cap    = new_cap;
}

/* ---------- push next x into heap ---------- */
static void push_next_x(void)
{
    uint32_t x = next_x_to_add++;
    uint32_t y = min_y_for_x(x);
    seq_t v;
    if (compute_v_checked(x, y, &v))
        heap_push((HeapNode){v, x, y});
    /* If it overflows, we skip it – such values are too large for seq_t. */
}

/* ---------- initialization ---------- */
static void init_state(void)
{
    min_cap   = 256;
    min_values = (seq_t*)calloc(min_cap, sizeof(seq_t));
    if (!min_values) abort();
    min_values[0] = (seq_t)1;          /* a(0) = 1 */

    heap_cap   = 256;
    heap_data  = (HeapNode*)malloc(heap_cap * sizeof(HeapNode));
    if (!heap_data) abort();
    heap_size = 0;

    /* Insert the first x */
    push_next_x();

    initialized = true;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (!initialized) init_state();

    /* Ensure the array can hold index n */
    if ((idx_t)n >= (idx_t)min_cap) {
        size_t new_cap = min_cap;
        while ((idx_t)new_cap <= (idx_t)n)
            new_cap = new_cap * 2 + 1;
        grow_min_values(new_cap);
    }

    if (n > max_needed) max_needed = n;

    /* Generate values until a(max_needed) is found */
    while (min_values[max_needed] == (seq_t)0) {
        /* Pop the smallest value from the heap */
        HeapNode cur = heap_pop();
        seq_t val = cur.v;

        /* Advance the stream for this x */
        seq_t nxt_v;
        if (compute_v_checked(cur.x, cur.y + 1, &nxt_v))
            heap_push((HeapNode){nxt_v, cur.x, cur.y + 1});

        /* Count all occurrences of this value (duplicates are adjacent) */
        idx_t occurrences = 1;
        while (heap_size > 0 && heap_data[0].v == val) {
            HeapNode dup = heap_pop();
            /* push next for the duplicate x */
            if (compute_v_checked(dup.x, dup.y + 1, &nxt_v))
                heap_push((HeapNode){nxt_v, dup.x, dup.y + 1});
            occurrences++;
        }

        /* Record minimal value for 'occurrences' if first time seen */
        if ((idx_t)occurrences >= (idx_t)min_cap) {
            size_t new_cap = min_cap;
            while ((idx_t)new_cap <= (idx_t)occurrences)
                new_cap = new_cap * 2 + 1;
            grow_min_values(new_cap);
        }
        if (min_values[occurrences] == (seq_t)0)
            min_values[occurrences] = val;

        /* Insert next x to keep the heap full of candidates */
        push_next_x();
    }

    return min_values[n];
}
