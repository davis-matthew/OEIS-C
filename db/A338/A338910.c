/*
 * A338910  Numbers of the form prime(x) * prime(y) where x and y are both
 *          odd.
 * https://oeis.org/A338910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth smallest distinct product of two odd‑indexed primes
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* small sieve to generate all primes up to a limit                  */
/* ------------------------------------------------------------------ */
static void sieve_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    if (limit < 2) {
        *primes_out = NULL;
        *cnt_out    = 0;
        return;
    }

    bool *sieve = (bool *)calloc(limit + 1, sizeof(bool));
    if (!sieve)
        return;                     /* allocation failed – caller will abort */

    size_t   count = 0;
    uint64_t *pr   = NULL;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            ++count;
            /* grow storage if necessary */
            uint64_t *tmp = (uint64_t *)realloc(pr, count * sizeof(uint64_t));
            if (!tmp) {
                free(sieve);
                free(pr);
                *primes_out = NULL;
                *cnt_out    = 0;
                return;
            }
            pr   = tmp;
            pr[count - 1] = (uint64_t)i;

            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    sieve[j] = true;
            }
        }
    }
    free(sieve);
    *primes_out = pr;
    *cnt_out    = count;
}

/* ------------------------------------------------------------------ */
/* min‑heap utilities                                               */
/* ------------------------------------------------------------------ */
typedef struct {
    uint64_t val;
    idx_t    i;
    idx_t    j;
} Node;

static void heap_swap(Node *a, Node *b)
{
    Node tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heap_sift_up(Node *heap, size_t idx)
{
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap[idx].val >= heap[parent].val)
            break;
        heap_swap(&heap[idx], &heap[parent]);
        idx = parent;
    }
}

static void heap_sift_down(Node *heap, size_t idx, size_t size)
{
    while (1) {
        size_t left  = (idx << 1) + 1;
        size_t right = left + 1;
        size_t smallest = idx;

        if (left < size && heap[left].val < heap[smallest].val)
            smallest = left;
        if (right < size && heap[right].val < heap[smallest].val)
            smallest = right;

        if (smallest == idx)
            break;
        heap_swap(&heap[idx], &heap[smallest]);
        idx = smallest;
    }
}

/* ------------------------------------------------------------------ */
/* a(n) – main routine                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1 – behaviour for 0 is undefined */
        return 0;

    /* ---------- how many odd‑indexed primes are required? ----------- */
    idx_t k = 0;
    while ((k * (k + 1) / 2) < n)
        ++k;                     /* smallest k with k(k+1)/2 >= n */

    /* ---------- find enough primes, then take the odd ones -------- */
    idx_t  limit = k * 20 + 100;  /* rough initial upper bound   */
    if (limit < 2000)
        limit = 2000;

    uint64_t *primes_all = NULL;
    size_t    primes_cnt = 0;
    size_t    odd_cnt    = 0;

    /* reuse local arrays – fully regenerated each call – trivial cost */
    while (1) {
        sieve_primes_upto(limit, &primes_all, &primes_cnt);
        if (primes_all == NULL)
            return 0;           /* allocation failure – abort on purpose */

        odd_cnt = (primes_cnt + 1) / 2;      /* ceil(primes_cnt/2) */
        if (odd_cnt >= (size_t)k)
            break;                    /* enough odd primes */

        /* not enough – increase limit and try again */
        free(primes_all);
        limit *= 2;
    }

    /* extract the odd‑indexed primes into a small array */
    uint64_t *odd = (uint64_t *)malloc(odd_cnt * sizeof(uint64_t));
    if (!odd) {
        free(primes_all);
        return 0;
    }
    size_t oi = 0;
    for (size_t pi = 0; pi < primes_cnt; ++pi)
        if ((pi & 1) == 0)          /* 0‑based: indices 0,2,4,… are odd positions */
            odd[oi++] = primes_all[pi];

    /* ---------- build initial heap of (i,i) nodes ----------------- */
    size_t heap_size = odd_cnt;
    Node *heap = (Node *)malloc(heap_size * sizeof(Node));
    if (!heap) {
        free(odd);
        free(primes_all);
        return 0;
    }
    for (idx_t i = 0; i < odd_cnt; ++i) {
        heap[i].i = i;
        heap[i].j = i;
        heap[i].val = (uint64_t)odd[i] * (uint64_t)odd[i];
    }
    for (size_t i = (heap_size >> 1); i-- > 0; )
        heap_sift_down(heap, i, heap_size);

    /* ---------- extract terms until we hit the n‑th ---------------- */
    seq_t  result = 0;
    idx_t  count  = 0;
    while (1) {
        Node min = heap[0];
        ++count;
        result = (seq_t)min.val;     /* cast only when returning */

        if (count == n) {            /* finished – free and return */
            free(heap);
            free(odd);
            free(primes_all);
            return result;
        }

        /* replace root with last element, shrink heap */
        heap[0] = heap[--heap_size];
        heap_sift_down(heap, 0, heap_size);

        /* enqueue next pair in the same row if available */
        if (min.j + 1 < odd_cnt) {
            Node nxt;
            nxt.i = min.i;
            nxt.j = min.j + 1;
            nxt.val = (uint64_t)odd[nxt.i] * (uint64_t)odd[nxt.j];
            heap[heap_size++] = nxt;
            heap_sift_up(heap, heap_size - 1);
        }
    }
}
