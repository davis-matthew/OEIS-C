/*
 * A365802  Numbers k such that A163511(k) is a fifth power.
 * https://oeis.org/A365802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0, a(2)=16, for each non‑zero term x the values
   2*x+1 and 32*x are also terms.  The sequence is the sorted
   closure under these two functions.  This implementation builds
   the sequence on‑demand by a min‑heap of pending candidates. */

#include <stddef.h> /* size_t */

/* Number of terms that are guaranteed to be produced by the test
   harness (< 10000).  The extra margin protects against future
   extensions. */
#define MAX_TERMS     20000
#define HEAP_CAPACITY (MAX_TERMS * 2 + 10)

/* Statically allocated storage: no dynamic allocation is used,
   therefore the code compiles both as C11 and C++17. */
static seq_t seq_arr[MAX_TERMS];   /* the generated sequence   */
static size_t seq_len = 0;          /* number of terms stored   */

static seq_t heap_arr[HEAP_CAPACITY]; /* min‑heap of pending terms */
static size_t heap_size = 0;             /* current heap size      */

/* Push a value onto the min‑heap.  The heap is 0‑based. */
static void heap_push(seq_t v)
{
    size_t i = heap_size++;
    heap_arr[i] = v;
    /* bubble up */
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap_arr[p] <= heap_arr[i])
            break;
        seq_t t = heap_arr[p];
        heap_arr[p] = heap_arr[i];
        heap_arr[i] = t;
        i = p;
    }
}

/* Remove and return the minimum element from the heap. */
static seq_t heap_pop(void)
{
    seq_t r = heap_arr[0];
    seq_t last = heap_arr[--heap_size];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1; /* left child */
        if (l >= heap_size)
            break;
        size_t rchild = l + 1;
        size_t child = (rchild < heap_size && heap_arr[rchild] < heap_arr[l])
                       ? rchild : l;
        if (heap_arr[child] >= last)
            break;
        heap_arr[i] = heap_arr[child];
        i = child;
    }
    heap_arr[i] = last;
    return r;
}

/* Initialise the sequence and heap on the first call. */
static void seq_init(void)
{
    /* seq[0] = a(1) = 0, seq[1] = a(2) = 16 */
    seq_arr[0] = (seq_t)0;
    seq_arr[1] = (seq_t)16;
    seq_len = 2;

    /* Children of the non‑zero term 16 are 33 and 512. */
    heap_push((seq_t)2 * seq_arr[1] + (seq_t)1); /* 33  */
    heap_push((seq_t)32 * seq_arr[1]);           /* 512 */
}

/* Public function -------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                   /* offset 1; unused below 1 */

    /* Lazily initialise data structures on the first call. */
    if (seq_len == 0)
        seq_init();

    /* Grow the sequence until we have at least n terms. */
    while (seq_len < (size_t)n) {
        /* The heap must be non‑empty because the generation rule
           ensures that each term produces new children. */
        seq_t v = heap_pop();               /* next smallest term      */

        seq_arr[seq_len++] = v;             /* append to sequence      */

        /* Produce the two children of v and insert them. */
        heap_push((seq_t)2 * v + (seq_t)1);
        heap_push((seq_t)32 * v);
    }

    return seq_arr[(size_t)n - 1];
}
