/*
 * A397367  Lexicographically earliest sequence of distinct prime powers p^m,
 *          m >= 1, such that for any prime p, any run of consecutive
 *          multiples of p has length exactly 2.
 * https://oeis.org/A397367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   lexicographically earliest sequence of distinct prime powers p^m
 *            (m ≥ 1) such that for any prime p every run of consecutive
 *            multiples of p has length exactly 2. (OEIS A397367)
 * Range:     u8 n <= 15, u16 n <= 121, u32 n <= 8522, u64 n <= 8522,
 *            u128 n <= 8522, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Prime sieve (up to a fixed limit) ----------------- */
static const idx_t PRIME_LIMIT = 400000;  /* enough for n ≤ 10000 */
static uint64_t *primes = NULL;           /* array of primes */
static size_t primes_cnt = 0;

static void init_primes(void)
{
    if (primes != NULL) return;          /* already initialized */

    bool *is_prime = (bool*)calloc(PRIME_LIMIT + 1, sizeof(bool));
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i) is_prime[i] = true;

    for (idx_t p = 2; p * p <= PRIME_LIMIT; ++p)
        if (is_prime[p])
            for (idx_t m = p * p; m <= PRIME_LIMIT; m += p)
                is_prime[m] = false;

    size_t cnt = 0;
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) ++cnt;

    primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    size_t pos = 0;
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) primes[pos++] = i;

    primes_cnt = cnt;
    free(is_prime);
}

/* ---------- Modular exponentiation on seq_t ------------------- */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1U) result *= base;
        exp >>= 1U;
        if (exp) base *= base;
    }
    return result;
}

/* ---------- Heap of pending first terms of pairs --------------- */
typedef struct {
    seq_t value;        /* first power of the next pair for a prime */
    idx_t prime_index;  /* index into primes[] */
    idx_t exp;          /* exponent 'e' such that value = p^e (e is odd) */
} Item;

static Item *heap = NULL;
static size_t heap_size = 0;
static size_t heap_cap = 0;

static void heap_push(seq_t val, idx_t pidx, idx_t exp)
{
    if (heap_size >= heap_cap) {
        size_t new_cap = (heap_cap ? heap_cap * 2 : 4);
        Item *tmp = (Item*)realloc(heap, new_cap * sizeof(Item));
        heap = tmp; heap_cap = new_cap;
    }
    heap[heap_size].value = val;
    heap[heap_size].prime_index = pidx;
    heap[heap_size].exp = exp;
    ++heap_size;
}

static Item heap_pop_min(void)
{
    size_t min_i = 0;
    Item min_val = heap[0];
    for (size_t i = 1; i < heap_size; ++i) {
        if (heap[i].value < min_val.value) {
            min_i = i;
            min_val = heap[i];
        }
    }
    heap[min_i] = heap[heap_size - 1];
    --heap_size;
    return min_val;
}

/* ---------- Cache for generated terms ------------------------ */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* enlarge cache to accommodate at least 'need' elements */
static void ensure_cache_capacity(idx_t need)
{
    if (cache_cap < need) {
        size_t new_cap = (cache_cap ? cache_cap * 2 : 4);
        while (new_cap < (size_t)need) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp; cache_cap = new_cap;
    }
}

/* ---------- Generation routine -------------------------------- */
static idx_t next_prime_idx = 0;   /* next prime to be considered */

static void generate_until(idx_t n)
{
    /* ensure primes are ready */
    init_primes();

    while (cache_len < n) {
        /* add eligible primes whose first power is smaller than any
           current pending first value */
        while (next_prime_idx < primes_cnt) {
            seq_t pval = (seq_t)primes[next_prime_idx];

            if (heap_size == 0) {
                heap_push(pval, next_prime_idx, (idx_t)1);
                ++next_prime_idx;
                break;
            }

            /* find current minimum in heap */
            seq_t current_min = heap[0].value;
            for (size_t i = 1; i < heap_size; ++i)
                if (heap[i].value < current_min) current_min = heap[i].value;

            if (pval < current_min) {
                heap_push(pval, next_prime_idx, (idx_t)1);
                ++next_prime_idx;
            } else {
                break;
            }
        }

        /* pop the smallest pending first value */
        Item top = heap_pop_min();

        seq_t first  = top.value;
        seq_t second = pow_seq((seq_t)primes[top.prime_index], top.exp + 1);

        /* store the pair */
        ensure_cache_capacity(cache_len + 2);
        cache[cache_len++] = first;
        cache[cache_len++] = second;

        /* schedule the next pair for the same prime */
        seq_t new_val = pow_seq((seq_t)primes[top.prime_index], top.exp + 2);
        heap_push(new_val, top.prime_index, top.exp + 2);
    }
}

/* ---------- Public sequence function -------------------------- */
seq_t a(idx_t n)
{
    if (n < 1U) return (seq_t)0;  /* undefined for zero index */

    /* ensure generated terms cover the requested index */
    generate_until(n);

    return cache[n - 1];
}
