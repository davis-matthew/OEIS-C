/*
 * A370206  Numbers j whose symmetric representation of sigma(j) consists of
 *          two copies of unimodal width pattern 121 separated by 0.
 * https://oeis.org/A370206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 22, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A370206
   a(n) = the n-th smallest integer of the form 2^k * p * q
   where k>0, p and q are distinct primes, p>2, p < 2^(k+1) <
          2^(k+1)*p < q.  The sequence is ordered by the integer value
   (ascending).  The implementation builds all admissible pairs
   (k,p) that can contribute to the first 10000 terms and merges
   the infinite lists of q‑values for each pair by a min‑heap. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/* static helpers for heap management                                      */
typedef struct {
    seq_t val;      /* 2^k * p * q  */
    unsigned k;     /* exponent of 2 */
    size_t p_idx;   /* index into primes[] for p   */
    size_t q_idx;   /* index into primes[] for q   */
} Candidate;

static void heap_siftup(Candidate *heap, size_t pos)
{
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (heap[parent].val <= heap[pos].val) break;
        Candidate tmp = heap[parent];
        heap[parent] = heap[pos];
        heap[pos] = tmp;
        pos = parent;
    }
}

static void heap_siftdown(Candidate *heap, size_t size, size_t pos)
{
    while (true) {
        size_t left = (pos << 1) + 1;
        if (left >= size) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < size && heap[right].val < heap[left].val)
            smallest = right;
        if (heap[pos].val <= heap[smallest].val) break;
        Candidate tmp = heap[pos];
        heap[pos] = heap[smallest];
        heap[smallest] = tmp;
        pos = smallest;
    }
}

static void heap_push(Candidate *heap, size_t *size, Candidate val)
{
    heap[*size] = val;
    heap_siftup(heap, *size);
    (*size)++;
}

static Candidate heap_pop(Candidate *heap, size_t *size)
{
    Candidate ret = heap[0];
    (*size)--;
    heap[0] = heap[*size];
    heap_siftdown(heap, *size, 0);
    return ret;
}

/* --------------------------------------------------------------------- */
/* prime sieve ----------------------------------------------------------- */
#define PRIME_LIMIT 5000000   /* sufficient for the first 10 000 terms */

static uint32_t *primes = NULL;     /* global array of primes */
static size_t primes_cnt = 0;       /* number of primes */

/* build primes once */
static void build_primes(void)
{
    if (primes != NULL) return;
    bool *sieve = (bool *)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    for (size_t i = 0; i <= PRIME_LIMIT; i++) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (size_t i = 2; i * i <= PRIME_LIMIT; i++) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i) sieve[j] = false;
        }
    }
    /* count primes */
    primes_cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; i++)
        if (sieve[i]) primes_cnt++;
    primes = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; i++) {
        if (sieve[i]) primes[idx++] = (uint32_t)i;
    }
    free(sieve);
}

/* first index of prime > x (binary search) */
static size_t first_prime_gt(uint32_t x)
{
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;   /* may equal primes_cnt, meaning none */
}

/* --------------------------------------------------------------------- */
/* initialize heap of all admissible (k,p) pairs that can appear in the
   first 10000 terms.  We use KMAX = 12, and skip pairs whose minimal
   q is beyond PRIME_LIMIT, because their values are far larger than
   needed. */
#define KMAX 12

static Candidate *heap = NULL;
static size_t heap_sz = 0;
static bool heap_inited = false;

static void init_heap(void)
{
    if (heap_inited) return;
    build_primes();

    /* estimate max heap size */
    size_t est = 0;
    for (unsigned k = 1; k <= KMAX; k++) {
        uint32_t limit = (uint32_t)(1U << (k + 1));
        for (size_t pi = 0; pi < primes_cnt; pi++) {
            uint32_t p = primes[pi];
            if (p <= 2) continue;
            if (p >= limit) break;
            uint64_t prod = (uint64_t)limit * (uint64_t)p;
            if (prod > (uint64_t)PRIME_LIMIT) continue;   /* q start beyond limit */
            est++;
        }
    }
    heap = (Candidate *)malloc(est * sizeof(Candidate));
    heap_sz = 0;

    /* precompute powers of 2 */
    seq_t pow2[KMAX + 1];
    pow2[0] = (seq_t)1;
    for (unsigned i = 1; i <= KMAX; i++) pow2[i] = pow2[i - 1] * (seq_t)2;

    for (unsigned k = 1; k <= KMAX; k++) {
        uint32_t limit = (uint32_t)(1U << (k + 1));
        for (size_t pi = 0; pi < primes_cnt; pi++) {
            uint32_t p = primes[pi];
            if (p <= 2) continue;
            if (p >= limit) break;
            uint64_t prod = (uint64_t)limit * (uint64_t)p;  /* 2^(k+1) * p */
            size_t q_start = first_prime_gt((uint32_t)prod);
            if (q_start >= primes_cnt) continue;
            Candidate c;
            c.val = pow2[k] * (seq_t)p * (seq_t)primes[q_start];
            c.k = k;
            c.p_idx = pi;
            c.q_idx = q_start;
            heap_push(heap, &heap_sz, c);
        }
    }
    heap_inited = true;
}

/* --------------------------------------------------------------------- */
/* memoization of sequence values --------------------------------------- */

static seq_t *memo = NULL;
static size_t memo_len = 0;
static size_t memo_cap = 0;

static void extend_memo(size_t needed)
{
    if (memo_cap >= needed) return;
    size_t newcap = memo_cap == 0 ? 128 : memo_cap * 2;
    if (newcap < needed) newcap = needed;
    seq_t *newmem = (seq_t *)realloc(memo, newcap * sizeof(seq_t));
    memo = newmem;
    memo_cap = newcap;
}

/* --------------------------------------------------------------------- */
/* public function ------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* safety, although n>=1 */

    init_heap();

    if (memo_len < (size_t)n) {
        extend_memo((size_t)n);
        for (; memo_len < (size_t)n; memo_len++) {
            Candidate cur = heap_pop(heap, &heap_sz);
            memo[memo_len] = cur.val;
            /* push next q for the same (k,p) */
            size_t next_q = cur.q_idx + 1;
            if (next_q < primes_cnt) {
                Candidate nc;
                nc.val = (seq_t)(1ULL << cur.k) * (seq_t)primes[cur.p_idx]
                         * (seq_t)primes[next_q];
                nc.k = cur.k;
                nc.p_idx = cur.p_idx;
                nc.q_idx = next_q;
                heap_push(heap, &heap_sz, nc);
            }
        }
    }
    return memo[n - 1];
}
