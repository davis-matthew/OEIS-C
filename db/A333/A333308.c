/*
 * A333308  Numbers that are the sum of two distinct terms of A003622 (1st
 *          column of the Wythoff array, A035513).
 * https://oeis.org/A333308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A333308 – sums of two distinct terms of the Beatty sequence
        A003622 (a(n) = floor( n * φ² ) – 1), unique, sorted.
    The implementation generates the Beatty terms on demand and
    extracts the ordered, deduplicated pair‑sums via a min‑heap.
    All arithmetic uses integer types only: 64‑bit indices, and
    128‑bit intermediates for Beatty generation.  Conversion to seq_t
    occurs only when the final value is needed.  The routine works for
    any seq_t/idx_t pair supplied by the build.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*----- helper: integer sqrt for 128‑bit numbers ----------------------------*/
static unsigned __int128 isqrt128(unsigned __int128 v)
{
    unsigned __int128 low  = 0;
    unsigned __int128 high = ((unsigned __int128)1) << 64;   /* sqrt(2¹²⁸) = 2⁶⁴ */
    while (low < high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq <= v)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;                           /* floor(sqrt(v)) */
}

/*----- Beatty sequence A003622 term (1‑based index) ----------------------*/
static seq_t beatty_term(idx_t k)            /* k>=1, 1‑based */
{
    /* floor(sqrt(5*k*k)) using 128‑bit arithmetic */
    unsigned __int128 valSq     = (unsigned __int128)5 * (unsigned __int128)k * (unsigned __int128)k;
    unsigned __int128 sqrtFloor = isqrt128(valSq);               /* floor(k*sqrt5) */

    unsigned __int128 numerator = (unsigned __int128)3 * k + sqrtFloor;
    unsigned __int128 res       = numerator / 2;                  /* floor((3k + sqrtFloor)/2) */
    res >>= 0;                                                 /* alias for clarity */
    res -= 1;                                                 /* subtract the final 1 */

    return (seq_t)res;                                          /* cast to seq_t */
}

/*----- heap node --------------------------------------------------------*/
typedef struct {
    seq_t  sum;          /* a[i] + a[j] */
    idx_t  i;            /* 0‑based indices in Beatty array */
    idx_t  j;
} HeapNode;

/*----- static state ----------------------------------------------------*/
static seq_t *A      = NULL;      /* Beatty array, 0‑based */
static size_t capA   = 0, lenA   = 0;
static HeapNode *heap    = NULL;  /* min‑heap of sums */
static size_t    capHeap = 0, heapSize = 0;
static seq_t *data     = NULL;    /* resulting sequence values */
static size_t  capData = 0, lenData = 0;
static seq_t  lastSum;            /* last value emitted */
static bool   haveLast = false;

/*----- heap operations -------------------------------------------------*/
static void heap_push(const HeapNode *node)
{
    if (heapSize >= capHeap) {
        size_t newCap = (capHeap == 0) ? 8 : capHeap * 2;
        HeapNode *tmp = (HeapNode *)realloc(heap, newCap * sizeof(HeapNode));
        heap = tmp;
        capHeap = newCap;
    }
    size_t pos = heapSize++;
    heap[pos] = *node;
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (heap[pos].sum >= heap[parent].sum) break;
        HeapNode t = heap[pos];
        heap[pos] = heap[parent];
        heap[parent] = t;
        pos = parent;
    }
}

static HeapNode heap_pop(void)
{
    HeapNode root = heap[0];
    heap[0] = heap[--heapSize];
    size_t pos = 0;
    while (true) {
        size_t left  = pos * 2 + 1;
        size_t right = left + 1;
        size_t smallest = pos;
        if (left  < heapSize && heap[left].sum  < heap[smallest].sum) smallest = left;
        if (right < heapSize && heap[right].sum < heap[smallest].sum) smallest = right;
        if (smallest == pos) break;
        HeapNode t = heap[pos];
        heap[pos] = heap[smallest];
        heap[smallest] = t;
        pos = smallest;
    }
    return root;
}

/*-----  Beatty array expansion ----------------------------------------*/
static void ensureBeattyUpTo(idx_t idx) /* idx is 0‑based index, ensure lenA>idx */
{
    while (lenA <= idx) {
        idx_t k = lenA + 1;                     /* 1‑based position */
        seq_t val = beatty_term(k);
        if (lenA >= capA) {
            size_t newCap = (capA == 0) ? 8 : capA * 2;
            seq_t *tmp = (seq_t *)realloc(A, newCap * sizeof(seq_t));
            A = tmp;
            capA = newCap;
        }
        A[lenA++] = val;

        /* push the new pair (lenA-2 , lenA-1) if possible */
        if (lenA >= 2) {
            HeapNode node;
            node.i   = lenA - 2;
            node.j   = lenA - 1;
            node.sum = A[node.i] + A[node.j];
            heap_push(&node);
        }
    }
}

/*-----  public sequence function ----------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1, but guard against 0 */

    while (lenData < n) {
        /* ensure we have at least two Beatty terms and heap is ready */
        if (lenA < 2) ensureBeattyUpTo(1);

        /* pop smallest sum */
        HeapNode cur   = heap_pop();
        seq_t val      = cur.sum;

        /* store only distinct values */
        if (!haveLast || val != lastSum) {
            if (lenData >= capData) {
                size_t newCap = (capData == 0) ? 8 : capData * 2;
                seq_t *tmp = (seq_t *)realloc(data, newCap * sizeof(seq_t));
                data = tmp;
                capData = newCap;
            }
            data[lenData++] = val;
            lastSum = val;
            haveLast = true;
        }

        /* push the next pair with the same i */
        idx_t newJ = cur.j + 1;
        ensureBeattyUpTo(newJ);
        HeapNode next;
        next.i   = cur.i;
        next.j   = newJ;
        next.sum = A[next.i] + A[next.j];
        heap_push(&next);
    }
    return data[n - 1];
}
