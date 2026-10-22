/*
 * A380146  Numbers that set records in A113901.
 * https://oeis.org/A380146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A380146
 *  Numbers that set records in A113901.
 *  The sequence a(n) consists of the minimal integers whose least
 *  prime signature has the property that the product
 *      omega(L(n)) * bigomega(L(n))
 *  sets a new record when n is that integer.
 *  The implementation scans the space of least prime signatures
 *  in increasing order using a min‑heap.  For each signature the
 *  product is computed from its exponent list, and a new record
 *  is stored whenever the product exceeds all previous products.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------  dynamic array for primes  -------------- */
#define PRIME_INIT_CAP 64
static uint64_t *primes = NULL;      /* array of primes (uint64_t)      */
static size_t   primes_cnt = 0;      /* number of primes stored        */
static uint64_t prime_candidate = 2; /* next candidate for primality   */

static bool is_prime_small(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < primes_cnt; ++i)
    {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

static void ensure_prime_index(size_t idx)
{
    if (!primes)
    {
        primes = (uint64_t*)malloc(PRIME_INIT_CAP * sizeof(uint64_t));
        primes_cnt = 0;
    }
    while (primes_cnt <= idx)
    {
        while (!is_prime_small(prime_candidate))
            ++prime_candidate;
        primes[primes_cnt++] = prime_candidate;
        prime_candidate += (prime_candidate == 2 ? 1 : 2);   /* next odd */
    }
}

/* ------------  heap for least prime signatures  ------------- */
typedef struct {
    seq_t   value;      /* current least prime signature value     */
    idx_t   k;          /* number of distinct primes in signature   */
    unsigned int last_exp; /* exponent of the last prime          */
    unsigned int sum_exp;  /* sum of all exponents (bigomega)      */
} HeapNode;

static HeapNode *heap   = NULL;   /* binary min‑heap  */
static size_t   heap_size = 0;
static size_t   heap_cap   = 0;

static void heap_push(const HeapNode *node)
{
    if (heap_size == heap_cap)
    {
        heap_cap = (heap_cap == 0) ? 64 : heap_cap * 2;
        heap = (HeapNode*)realloc(heap, heap_cap * sizeof(HeapNode));
    }
    size_t i = heap_size++;
    heap[i] = *node;
    while (i > 0)
    {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].value <= heap[i].value) break;
        HeapNode tmp = heap[parent];
        heap[parent] = heap[i];
        heap[i] = tmp;
        i = parent;
    }
}

static HeapNode heap_pop(void)
{
    HeapNode res = heap[0];
    heap[0] = heap[--heap_size];
    size_t i = 0;
    while (true)
    {
        size_t l = (i << 1) + 1;
        if (l >= heap_size) break;
        size_t r = l + 1;
        size_t smallest = l;
        if (r < heap_size && heap[r].value < heap[l].value) smallest = r;
        if (heap[i].value <= heap[smallest].value) break;
        HeapNode tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
    return res;
}

/* ------------  record array  ---------------------- */
static seq_t *record_vals = NULL;
static size_t record_len  = 0;
static size_t record_cap  = 0;

static void record_append(seq_t val)
{
    if (record_len == record_cap)
    {
        record_cap = (record_cap == 0) ? 64 : record_cap * 2;
        record_vals = (seq_t*)realloc(record_vals, record_cap * sizeof(seq_t));
    }
    record_vals[record_len++] = val;
}

/* ------------  initialization flag  --------- */
static bool initialized = false;

/* -----------------  main sequence function  ----------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, return 0 */

    if (!initialized)
    {
        /* set up initial data */
        ensure_prime_index(0);          /* prime[0] = 2   */

        /* record first term  */
        record_append((seq_t)1);
        seq_t max_product = (seq_t)0;   /* product for 1 */

        /* push the first least prime signature 2 */
        HeapNode first = { (seq_t)2, 1, 1, 1 };
        heap_push(&first);

        initialized = true;
    }

    static seq_t max_product = (seq_t)0;
    /* loop until we have enough record values */
    while (record_len < n)
    {
        if (heap_size == 0)
            break; /* should not happen */

        HeapNode cur = heap_pop();

        /* compute product omega * bigomega */
        seq_t prod = (seq_t)cur.k * (seq_t)cur.sum_exp;
        if (prod > max_product)
        {
            record_append(cur.value);
            max_product = prod;
        }

        /* generate children */
        /* 1. increase last exponent (if any) */
        if (cur.k > 0)
        {
            ensure_prime_index(cur.k - 1);
            HeapNode ch;
            ch.value    = cur.value * (seq_t)primes[cur.k - 1];
            ch.k        = cur.k;
            ch.last_exp = cur.last_exp + 1;
            ch.sum_exp  = cur.sum_exp + 1;
            heap_push(&ch);
        }

        /* 2. add new prime with exponent 1 */
        ensure_prime_index(cur.k);
        HeapNode ch;
        ch.value    = cur.value * (seq_t)primes[cur.k];
        ch.k        = cur.k + 1;
        ch.last_exp = 1;
        ch.sum_exp  = cur.sum_exp + 1;
        heap_push(&ch);
    }

    return record_vals[n - 1];
}
