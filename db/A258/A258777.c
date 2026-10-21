/*
 * A258777  Number of points of projective spaces on finite fields.
 * https://oeis.org/A258777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 6262, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the sequence of numbers of the form (p^(k*n)-1)/(p^k-1),
   i.e. the cardinality of projective spaces over finite fields.
   The algorithm merges the monotone sequences
   1, q+1, q*(q+1)+1, ... for every prime power base q = p^k. */
#include <stdlib.h>
#include <stdint.h>

static void generate_primes(uint64_t limit, uint64_t **out, size_t *cnt) {
    if (limit < 2) {
        *out = NULL;
        *cnt = 0;
        return;
    }
    size_t sz = limit + 1;
    char *is_composite = (char *)calloc(sz, 1);
    size_t max_primes = sz / 10 + 2;
    uint64_t *primes = (uint64_t *)malloc(max_primes * sizeof(uint64_t));
    size_t count = 0;

    for (uint64_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            if (count == max_primes) {
                max_primes *= 2;
                primes = (uint64_t *)realloc(primes, max_primes * sizeof(uint64_t));
            }
            primes[count++] = i;
            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    is_composite[j] = 1;
            }
        }
    }
    free(is_composite);
    *out = primes;
    *cnt = count;
}

typedef struct {
    seq_t current;
    uint64_t q;
    seq_t val;
} Node;

static Node *heap = NULL;
static size_t heap_size = 0;
static size_t heap_cap = 0;

static void heap_push(Node n) {
    if (heap_size == heap_cap) {
        heap_cap = (heap_cap ? heap_cap * 2 : 8);
        heap = (Node *)realloc(heap, heap_cap * sizeof(Node));
    }
    size_t i = heap_size++;
    heap[i] = n;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= heap[i].val) break;
        Node t = heap[p];
        heap[p] = heap[i];
        heap[i] = t;
        i = p;
    }
}

static Node heap_pop_min(void) {
    Node res = heap[0];
    heap[0] = heap[--heap_size];
    size_t i = 0;
    for (;;) {
        size_t l = (i << 1) + 1, r = l + 1, s = i;
        if (l < heap_size && heap[l].val < heap[s].val) s = l;
        if (r < heap_size && heap[r].val < heap[s].val) s = r;
        if (s == i) break;
        Node t = heap[s];
        heap[s] = heap[i];
        heap[i] = t;
        i = s;
    }
    return res;
}

static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

static uint64_t current_limit = 0;

static void add_node(uint64_t q) {
    Node n;
    n.q = q;
    n.current = (seq_t)1;
    n.val = (seq_t)(q + 1);          /* first value after 1  */
    heap_push(n);
}

static void extend_limit(uint64_t new_limit) {
    if (new_limit <= current_limit) return;
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    generate_primes(new_limit, &primes, &prime_cnt);
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t val = p;
        while (val <= new_limit) {
            if (val > current_limit)
                add_node(val);
            if (val > new_limit / p) break;
            val *= p;
        }
    }
    free(primes);
    current_limit = new_limit;
}

static int initialized = 0;

static void a_init(void) {
    if (initialized) return;
    seq_cap = 16;
    seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    seq[0] = (seq_t)1;              /* a(1) */
    seq_len = 1;
    heap_cap = 8;
    heap = (Node *)malloc(heap_cap * sizeof(Node));
    heap_size = 0;
    current_limit = 0;
    extend_limit(200000); /* initial guess for prime powers */
    initialized = 1;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    a_init();
    while (seq_len < n) {
        if (heap_size == 0) {
            uint64_t new_limit = current_limit * 2 + 1000;
            if (new_limit <= current_limit) new_limit = current_limit + 1000;
            extend_limit(new_limit);
            if (heap_size == 0) break;
        }
        Node node = heap_pop_min();
        seq_t val = node.val;
        if (seq_len == 0 || val != seq[seq_len - 1]) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = val;
        }
        seq_t next_val = val * (seq_t)node.q + 1;
        node.current = val;
        node.val = next_val;
        heap_push(node);
    }
    return seq[n - 1];
}
