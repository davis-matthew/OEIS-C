/*
 * A282844  n-th even semiprime plus n-th odd semiprime.
 * https://oeis.org/A282844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A100484(n) + A046315(n)
 * Range:     u8 n <= 20, u16 n <= 2813, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A100484(n) = nth even semiprime = 2 * nth prime */
/* A046315(n) = nth odd semiprime = product of two odd primes in increasing order */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Utilities supplied by the build */
#include "../utils/build_primes_upto.h"

/* Global storage for the prime table (including 2) */
static uint64_t *prime_list   = NULL;
static size_t   prime_count   = 0;
static bool     primes_loaded = false;

/* Build the global prime table once */
static void init_prime_table(void)
{
    if (primes_loaded) return;
    const idx_t LIMIT = 2000000;            /* enough for all required n */
    build_primes_upto(LIMIT, &prime_list, &prime_count);
    primes_loaded = true;
}

/* Return the nth prime (1‑based, primes[0] = 2) */
static seq_t nth_prime(idx_t n)
{
    init_prime_table();
    return (seq_t)prime_list[n - 1];
}

/* Count how many odd semiprimes are <= x, stop early if reaching target_n */
static idx_t count_odd_semiprimes(seq_t x, idx_t target_n)
{
    idx_t cnt = 0;
    idx_t pc   = (idx_t)prime_count;
    for (idx_t i = 1; i < pc; ++i) {          /* skip 2 to keep primes odd */
        seq_t p = (seq_t)prime_list[i];
        if (p * p > x) break;
        for (idx_t j = i; j < pc; ++j) {
            seq_t prod = p * (seq_t)prime_list[j];
            if (prod > x) break;
            ++cnt;
            if (cnt >= target_n) return cnt;
        }
    }
    return cnt;
}

/* Node used in the min‑heap for generating odd semiprimes */
typedef struct {
    seq_t prod;
    idx_t i;
    idx_t j;
} heap_node_t;

/* Generate the nth odd semiprime using a min‑heap */
static seq_t nth_odd_semiprime(idx_t n)
{
    init_prime_table();
    idx_t pc = (idx_t)prime_count;

    /* 1) Find a high enough bound where at least n odd semiprimes are <= bound */
    seq_t low = 0, high = 2;
    while (count_odd_semiprimes(high, n) < n) high = high * 2;

    while (low + 1 < high) {
        seq_t mid = low + ((high - low) >> 1);
        if (count_odd_semiprimes(mid, n) >= n) high = mid;
        else low = mid;
    }
    seq_t bound = high;                      /* smallest x with >= n odd semiprimes */

    /* 2) Build a heap of initial pairs (p_i * p_i, i, i) for primes p_i^2 <= bound */
    static heap_node_t *heap  = NULL;
    static size_t       heap_cap = 0;
    if (heap_cap < pc) {
        if (heap) free(heap);
        heap = (heap_node_t *)malloc(sizeof(heap_node_t) * pc);
        heap_cap = pc;
    }
    size_t heap_size = 0;
    for (idx_t i = 1; i < pc; ++i) {           /* start from prime = 3 */
        seq_t p = (seq_t)prime_list[i];
        if (p * p > bound) break;
        heap[heap_size].prod = p * p;
        heap[heap_size].i    = i;
        heap[heap_size].j    = i;
        ++heap_size;
    }

    /* heapify */
    for (size_t k = heap_size / 2; k-- > 0; ) {
        size_t pos = k;
        while (1) {
            size_t left   = pos * 2 + 1;
            size_t right  = left + 1;
            size_t smallest = pos;
            if (left  < heap_size && heap[left].prod < heap[smallest].prod) smallest = left;
            if (right < heap_size && heap[right].prod < heap[smallest].prod) smallest = right;
            if (smallest == pos) break;
            heap_node_t tmp = heap[pos];
            heap[pos] = heap[smallest];
            heap[smallest] = tmp;
            pos = smallest;
        }
    }

    /* 3) Extract the smallest n times */
    seq_t nth_value = 0;
    for (idx_t taken = 1; taken <= n; ++taken) {
        heap_node_t smallest = heap[0];
        nth_value = smallest.prod;

        if (taken == n) break;

        /* Replace root with the last element and heapify down */
        heap[0] = heap[--heap_size];

        size_t pos = 0;
        while (1) {
            size_t left   = pos * 2 + 1;
            size_t right  = left + 1;
            size_t smallest = pos;
            if (left  < heap_size && heap[left].prod < heap[smallest].prod) smallest = left;
            if (right < heap_size && heap[right].prod < heap[smallest].prod) smallest = right;
            if (smallest == pos) break;
            heap_node_t tmp = heap[pos];
            heap[pos] = heap[smallest];
            heap[smallest] = tmp;
            pos = smallest;
        }

        /* Push the next pair (i, j+1) if it stays within bound */
        idx_t ii = smallest.i;
        idx_t jj = smallest.j + 1;
        if (jj < pc) {
            seq_t prod = (seq_t)prime_list[ii] * (seq_t)prime_list[jj];
            if (prod <= bound) {
                /* Insert new node at the end and bubble up */
                heap[heap_size].prod = prod;
                heap[heap_size].i    = ii;
                heap[heap_size].j    = jj;
                ++heap_size;

                size_t pos_up = heap_size - 1;
                while (pos_up > 0) {
                    size_t parent = (pos_up - 1) / 2;
                    if (heap[pos_up].prod >= heap[parent].prod) break;
                    heap_node_t tmp = heap[pos_up];
                    heap[pos_up] = heap[parent];
                    heap[parent] = tmp;
                    pos_up = parent;
                }
            }
        }
    }
    return nth_value;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n < 1 */

    /* Even semiprime = 2 * nth prime */
    seq_t even = (seq_t)2 * nth_prime(n);

    /* Odd semiprime = nth odd semiprime */
    seq_t odd = nth_odd_semiprime(n);

    return even + odd;
}
