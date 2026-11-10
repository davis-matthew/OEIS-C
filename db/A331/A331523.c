/*
 * A331523  a(n) is the least positive k such that A100271(n) - k^3 is a
 *          fourth power.
 * https://oeis.org/A331523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k>0 with A100271(n) - k^3 a fourth power; found by
 *            searching a downward from floor(P^(1/4)) for P - a^4 a cube
 *            (largest a gives least k)
 * Range:     u8 n <= 6, u16 n <= 63, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9979 (2026-11-08)
 * Helpers:   icbrt_seq, is_prime_u64, isqrt_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"
#include "../utils/icbrt_seq.h"
#include "../utils/isqrt_seq.h"

static inline unsigned __int128 A100271_a4_plus_b3(uint64_t a, uint64_t b)
{
    unsigned __int128 a2 = (unsigned __int128)a * a;
    unsigned __int128 a4 = a2 * a2;
    unsigned __int128 b2 = (unsigned __int128)b * b;
    unsigned __int128 b3 = b2 * b;
    return a4 + b3;
}

typedef struct {
    unsigned __int128 val;
    uint64_t          a;
    uint64_t          b;
} A100271_Node;

#define A100271_INIT_CAP 1024
#define A100271_MAX_A    20000

static A100271_Node *A100271_heap = NULL;
static size_t A100271_heap_size = 0;
static size_t A100271_heap_cap = 0;

static seq_t *A100271_primes = NULL;
static size_t A100271_p_cnt = 0;
static size_t A100271_p_cap = 0;

static uint64_t A100271_next_a = 2;
static uint64_t A100271_last_prime_val = 0;

static inline bool A100271_node_lt(const A100271_Node *x, const A100271_Node *y)
{
    if (x->val != y->val) return x->val < y->val;
    if (x->a   != y->a  ) return x->a   < y->a;
    return x->b < y->b;
}

static inline void A100271_swap(A100271_Node *x, A100271_Node *y)
{
    A100271_Node tmp = *x; *x = *y; *y = tmp;
}

static void A100271_heap_push(A100271_Node n)
{
    if (A100271_heap_size == A100271_heap_cap) {
        size_t new_cap = A100271_heap_cap ? A100271_heap_cap * 2 : A100271_INIT_CAP;
        A100271_Node *tmp = (A100271_Node*)realloc(A100271_heap, new_cap * sizeof(A100271_Node));
        if (!tmp) exit(EXIT_FAILURE);
        A100271_heap = tmp; A100271_heap_cap = new_cap;
    }
    A100271_heap[A100271_heap_size++] = n;
    size_t i = A100271_heap_size - 1;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (!A100271_node_lt(&A100271_heap[i], &A100271_heap[p])) break;
        A100271_swap(&A100271_heap[i], &A100271_heap[p]);
        i = p;
    }
}

static A100271_Node A100271_heap_pop_min(void)
{
    A100271_Node ret = A100271_heap[0];
    A100271_heap[0] = A100271_heap[--A100271_heap_size];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        if (l >= A100271_heap_size) break;
        size_t r = l + 1;
        size_t child = (r < A100271_heap_size && A100271_node_lt(&A100271_heap[r], &A100271_heap[l])) ? r : l;
        if (!A100271_node_lt(&A100271_heap[child], &A100271_heap[i])) break;
        A100271_swap(&A100271_heap[child], &A100271_heap[i]);
        i = child;
    }
    return ret;
}

static void A100271_append_prime(seq_t v)
{
    if (A100271_p_cnt == A100271_p_cap) {
        size_t new_cap = A100271_p_cap ? A100271_p_cap * 2 : A100271_INIT_CAP;
        seq_t *tmp = mem_resize(seq_t, A100271_primes, new_cap);
        if (!tmp) exit(EXIT_FAILURE);
        A100271_primes = tmp; A100271_p_cap = new_cap;
    }
    A100271_primes[A100271_p_cnt++] = v;
}

static seq_t A100271_a(idx_t n)
{
    if (!A100271_heap) {
        A100271_heap = (A100271_Node*)malloc(A100271_INIT_CAP * sizeof(A100271_Node));
        if (!A100271_heap) exit(EXIT_FAILURE);
        A100271_heap_cap = A100271_INIT_CAP;
    }
    if (!A100271_primes) {
        A100271_primes = mem_new(seq_t, A100271_INIT_CAP);
        if (!A100271_primes) exit(EXIT_FAILURE);
        A100271_p_cap = A100271_INIT_CAP;
    }
    if (A100271_heap_size == 0) {
        A100271_heap_push((A100271_Node){A100271_a4_plus_b3(1, 1), 1, 1});
    }
    while (A100271_p_cnt < (size_t)n) {
        A100271_Node cur = A100271_heap_pop_min();
        if ((cur.val >> 64) == 0) {
            uint64_t val64 = (uint64_t)cur.val;
            if (is_prime_u64(val64) && val64 != A100271_last_prime_val) {
                A100271_append_prime((seq_t)val64);
                A100271_last_prime_val = val64;
            }
        }
        A100271_heap_push((A100271_Node){A100271_a4_plus_b3(cur.a, cur.b + 1), cur.a, cur.b + 1});
        if (A100271_next_a <= A100271_MAX_A) {
            A100271_heap_push((A100271_Node){A100271_a4_plus_b3(A100271_next_a, 1), A100271_next_a, 1});
            ++A100271_next_a;
        }
    }
    return A100271_primes[n - 1];
}

seq_t a(idx_t n)
{
    seq_t P = A100271_a(n);
    seq_t r = isqrt_seq(isqrt_seq(P));
    seq_t x;
    for (x = r + 1; x > 0; ) {
        --x;
        seq_t m = P - x * x * x * x;
        if (m <= 0) continue;
        seq_t c = icbrt_seq(m);
        while (c * c * c > m) --c;
        while ((c + 1) * (c + 1) * (c + 1) <= m) ++c;
        if (c * c * c == m) return c;
    }
    return 0;
}
