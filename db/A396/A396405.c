/*
 * A396405  A sequence constructed by greedily sampling the probability
 *          distribution given by 1/log_2(i+1)^2 - 1/log_2(i+2)^2 to minimize
 *          ratio discrepancy.
 * https://oeis.org/A396405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Greedy selection from the heavy‑tail distribution
   p(k) = 1/log₂(k+1)² – 1/log₂(k+2)².
   At each step the value (k+1) with the smallest
   ratio (c(k)+1)/p(k) is chosen, where c(k) is the number
   of occurrences of (k+1) so far.  a(n) is the n‑th chosen value. */
/* ------------------------------------------------------------------ */
/*  Includes and type definitions ------------------------------------- */
/* ------------------------------------------------------------------ */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/*  Constants -------------------------------------------------------- */
/* ------------------------------------------------------------------ */
#define MAX_K 30000U          /* maximum k index supported          */
#define MAX_SEQ 40000         /* maximum number of terms we store   */

/* ------------------------------------------------------------------ */
/*  Global data ------------------------------------------------------ */
/* ------------------------------------------------------------------ */
static uint64_t count_val[MAX_K];      /* counters c(k)                    */
static double   prob_k[MAX_K];         /* probability p(k)                 */
static double   next_val[MAX_K];       /* (c+1)/p(k) key for heap          */
static unsigned heap_table[MAX_K];      /* binary heap of k indices         */
static size_t   heap_sz = 0;            /* number of elements in the heap   */
static bool     heap_inited = false;    /* has the heap been initialised?   */

static seq_t   seq_arr[MAX_SEQ];       /* memoised sequence values         */
static size_t  seq_len = 0;             /* number of terms stored           */

/* ------------------------------------------------------------------ */
/*  Heap utilities --------------------------------------------------- */
/* ------------------------------------------------------------------ */
static inline int heap_cmp(unsigned i, unsigned j)
{
    double di = next_val[i];
    double dj = next_val[j];
    if (di < dj) return -1;
    if (di > dj) return  1;
    return 0;
}

static void heap_sift_up(size_t pos)
{
    while (pos > 0)
    {
        size_t parent = (pos - 1) >> 1;
        if (heap_cmp(heap_table[pos], heap_table[parent]) >= 0) break;
        unsigned tmp = heap_table[pos];
        heap_table[pos] = heap_table[parent];
        heap_table[parent] = tmp;
        pos = parent;
    }
}

static void heap_sift_down(size_t pos)
{
    while (true)
    {
        size_t left = (pos << 1) + 1;
        if (left >= heap_sz) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < heap_sz && heap_cmp(heap_table[right], heap_table[left]) < 0)
            smallest = right;
        if (heap_cmp(heap_table[smallest], heap_table[pos]) >= 0) break;
        unsigned tmp = heap_table[pos];
        heap_table[pos] = heap_table[smallest];
        heap_table[smallest] = tmp;
        pos = smallest;
    }
}

/* ------------------------------------------------------------------ */
/*  Initialisation --------------------------------------------------- */
/* ------------------------------------------------------------------ */
static void heap_init(void)
{
    if (heap_inited) return;
    for (unsigned k = 0; k < MAX_K; ++k)
    {
        double l1 = log2((double)(k + 2));
        double l2 = log2((double)(k + 3));
        prob_k[k]   = 1.0/(l1*l1) - 1.0/(l2*l2);
        count_val[k] = 0ULL;
        next_val[k]  = 1.0/prob_k[k];
        heap_table[k] = k;
    }
    heap_sz = MAX_K;
    for (size_t i = (heap_sz >> 1); i-- > 0; ) heap_sift_down(i);
    heap_inited = true;
}

/* ------------------------------------------------------------------ */
/*  Heap pop & update ------------------------------------------------ */
/* ------------------------------------------------------------------ */
static unsigned heap_pop_and_update(void)
{
    unsigned top = heap_table[0];
    heap_table[0] = heap_table[--heap_sz];
    heap_sift_down(0);

    count_val[top] += 1ULL;
    next_val[top] = ((double)(count_val[top] + 1.0)) / prob_k[top];

    heap_table[heap_sz++] = top;
    heap_sift_up(heap_sz - 1);
    return top;
}

/* ------------------------------------------------------------------ */
/*  Public sequence function ---------------------------------------- */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* under‑flow safety */

    heap_init();

    while (seq_len < (size_t)n)
    {
        unsigned chosen = heap_pop_and_update();   /* 0‑based k */
        seq_arr[seq_len++] = (seq_t)(chosen + 1);  /* value is (k+1) */
    }
    return seq_arr[n - 1];
}
