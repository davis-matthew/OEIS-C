/*
 * A373899  Semiprimes q*p such that q^p == p (mod (q - p)), where q > p.
 * https://oeis.org/A373899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th smallest product of two primes (p<q) such that
 *            q^p == p (mod (q - p)). This is OEIS A373899. Supports any
 *            build‑dependent integer type seq_t. The sequence is generated
 *            lazily with a min‑heap over all primes up to a hard‑coded limit
 * Range:     u8 none, u16 none, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* -------------------------------------------------------------------------- */
/*  Helper: modular exponentiation for 64‑bit modulus, using 128‑bit multiplication. */
static inline uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp > 0) {
        if (exp & 1)
            result = (uint64_t)(((__uint128_t)result * b) % mod);
        b = (uint64_t)(((__uint128_t)b * b) % mod);
        exp >>= 1;
    }
    return result;
}

/* -------------------------------------------------------------------------- */
/*  Sieve of Eratosthenes to generate all primes up to a given limit. */
static void sieve_primes(uint64_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    memset(is_prime, 1, (limit + 1) * sizeof(bool));
    is_prime[0] = is_prime[1] = false;
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }
    /* Count primes */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;
    /* Allocate array */
    uint64_t *primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[idx++] = i;
    free(is_prime);
    *primes_out = primes;
    *cnt_out = cnt;
}

/* -------------------------------------------------------------------------- */
/*  Min‑heap node: stores product and indices of primes p (index) and q (index). */
typedef struct {
    seq_t prod;
    size_t p_index;   /* index of the smaller prime p */
    size_t q_index;   /* index of the larger prime q */
} heap_node_t;

/* -------------------------------------------------------------------------- */
/*  Heap operations (min‑heap on prod). */
static heap_node_t *heap = NULL;
static size_t heap_size = 0;

/* min‑heap insertion */
static void heap_push(heap_node_t node)
{
    if (!heap) heap = (heap_node_t *)malloc(20000 * sizeof(heap_node_t)); /* sufficient for our limit */
    heap[heap_size] = node;
    size_t i = heap_size++;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[i].prod >= heap[parent].prod) break;
        heap_node_t tmp = heap[i];
        heap[i] = heap[parent];
        heap[parent] = tmp;
        i = parent;
    }
}

/* pop minimum element */
static heap_node_t heap_pop(void)
{
    heap_node_t top = heap[0];
    heap[0] = heap[--heap_size];
    size_t i = 0;
    while (1) {
        size_t left = (i << 1) + 1;
        if (left >= heap_size) break;
        size_t right = left + 1;
        size_t child = (right < heap_size && heap[right].prod < heap[left].prod) ? right : left;
        if (heap[child].prod >= heap[i].prod) break;
        heap_node_t tmp = heap[i];
        heap[i] = heap[child];
        heap[child] = tmp;
        i = child;
    }
    return top;
}

/* -------------------------------------------------------------------------- */
/*  Global state ------------------------------------------------------------- */
static bool initialized = false;
static uint64_t *prime_list = NULL;        /* array of primes */
static size_t prime_cnt = 0;               /* number of primes */
static size_t *next_q_index = NULL;        /* next candidate q index for each p */

/* Sequence cache */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* -------------------------------------------------------------------------- */
/*  Initialize global data, called once on first use. */
static void init_sequence(void)
{
    if (initialized) return;
    initialized = true;

    /* ------------------ 1. Generate primes up to a fixed limit ----------------- */
    const uint64_t PRIME_LIMIT = 200000;  /* chosen to provide enough terms for n up to ~10k */
    sieve_primes(PRIME_LIMIT, &prime_list, &prime_cnt);

    /* ------------------ 2. Allocate per‑prime state ------------------ */
    next_q_index = (size_t *)malloc(prime_cnt * sizeof(size_t));
    for (size_t i = 0; i < prime_cnt; ++i)
        next_q_index[i] = prime_cnt;  /* sentinel value meaning "none" */

    /* ------------------ 3. Prepare heap with first valid pair of each prime ------- */
    for (size_t p = 0; p < prime_cnt; ++p) {
        size_t q = p + 1;
        while (q < prime_cnt) {
            uint64_t d = prime_list[q] - prime_list[p];
            if (mod_pow_uint64(prime_list[p], prime_list[p], d) == prime_list[p] % d) {
                seq_t prod = (seq_t)prime_list[p] * prime_list[q];
                heap_push((heap_node_t){prod, p, q});
                next_q_index[p] = q + 1;
                break;
            }
            ++q;
        }
        /* if no q found, sentinel remains */
    }

    /* ------------------ 4. Allocate initial cache ------------------ */
    seq_cap = 1024;
    seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
}

/* -------------------------------------------------------------------------- */
/*  Append to sequence cache, growing if necessary. */
static void seq_append(seq_t val)
{
    if (seq_len == seq_cap) {
        seq_cap *= 2;
        seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
    }
    seq_cache[seq_len++] = val;
}

/* -------------------------------------------------------------------------- */
/*  Main function: a(n) -------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_sequence();
    if (n == 0) return 0;   /* undefined, but safe placeholder */

    while (seq_len < (size_t)n) {
        heap_node_t top = heap_pop();
        seq_append(top.prod);

        /* Find next valid q for this p */
        size_t p = top.p_index;
        size_t q = next_q_index[p];
        while (q < prime_cnt) {
            uint64_t d = prime_list[q] - prime_list[p];
            if (mod_pow_uint64(prime_list[p], prime_list[p], d) == prime_list[p] % d) {
                seq_t prod = (seq_t)prime_list[p] * prime_list[q];
                heap_push((heap_node_t){prod, p, q});
                next_q_index[p] = q + 1;
                break;
            }
            ++q;
        }
        /* if q reached end, nothing to push; next_q_index[p] stays prime_cnt */
    }

    return seq_cache[(size_t)n - 1];
}
