/*
 * A385237  Smallest x such that x^3+y^3 = A004999(n), x and y are nonnegative
 *          integers.
 * https://oeis.org/A385237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A385237: a(n) = smallest x such that x³ + y³ = A004999(n) */
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    seq_t sum;
    idx_t x;
    idx_t y;
} HeapNode;

static bool initialized              = false;
static seq_t *cube_arr               = NULL;
static HeapNode *heap                = NULL;
static size_t heap_size              = 0;
static const idx_t MAX_X             = 2000;      /* safe upper bound for n ≤ 10000 */
static seq_t last_sum;
static bool  first_sum_flag          = true;

/* storage for the sequence values */
#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> result_vec;
#else
static seq_t *result_arr = NULL;
static size_t result_cap = 0;
#endif
static size_t result_len          = 0;

/* push an element onto the min‑heap (by sum) */
static void heap_push(HeapNode elem)
{
    size_t i = heap_size++;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].sum <= elem.sum) break;
        heap[i] = heap[parent];
        i = parent;
    }
    heap[i] = elem;
}

/* pop the element with smallest sum */
static HeapNode heap_pop(void)
{
    HeapNode ret = heap[0];
    heap[0] = heap[--heap_size];
    size_t i = 0;
    while (1) {
        size_t left = (i << 1) + 1;
        if (left >= heap_size) break;
        size_t right = left + 1;
        size_t best = left;
        if (right < heap_size && heap[right].sum < heap[left].sum)
            best = right;
        if (heap[i].sum <= heap[best].sum) break;
        HeapNode tmp = heap[i];
        heap[i] = heap[best];
        heap[best] = tmp;
        i = best;
    }
    return ret;
}

/* build the array of at least n terms */
static void extend_terms_to(idx_t n)
{
    if (!initialized) {
        /* pre‑compute cube values */
        cube_arr = (seq_t *)malloc(MAX_X * sizeof(seq_t));
        for (idx_t i = 0; i < MAX_X; ++i) {
            seq_t ii = (seq_t)i;
            cube_arr[i] = ii * ii * ii;
        }
        /* initialise the heap with x = y = 0…MAX_X‑1 */
        heap = (HeapNode *)malloc(MAX_X * sizeof(HeapNode));
        heap_size = 0;
        for (idx_t x = 0; x < MAX_X; ++x) {
            HeapNode node;
            node.x = x;
            node.y = x;
            node.sum = cube_arr[x] + cube_arr[x];
            heap_push(node);
        }
        initialized = true;
        first_sum_flag = true;
    }

    while (result_len < (size_t)n) {
        HeapNode node = heap_pop();

        if (first_sum_flag || node.sum != last_sum) {
#ifdef __cplusplus
            result_vec.push_back((seq_t)node.x);
#else
            if (result_len >= result_cap) {
                size_t new_cap = result_cap ? (result_cap << 1) : 1;
                seq_t *new_arr = (seq_t *)realloc(result_arr, new_cap * sizeof(seq_t));
                result_arr = new_arr;
                result_cap = new_cap;
            }
            result_arr[result_len] = (seq_t)node.x;
#endif
            result_len++;
            last_sum = node.sum;
            first_sum_flag = false;
        }

        /* advance to next y for this x */
        node.y++;
        if (node.y < MAX_X) {
            node.sum = cube_arr[node.x] + cube_arr[node.y];
            heap_push(node);
        }
        /* when node.y reaches MAX_X the entry is not re‑inserted */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n < 1 */
    extend_terms_to(n);
#ifdef __cplusplus
    return result_vec[(size_t)(n - 1)];
#else
    return result_arr[(size_t)(n - 1)];
#endif
}
