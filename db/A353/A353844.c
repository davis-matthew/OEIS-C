/*
 * A353844  Starting with the multiset of prime indices of n, repeatedly take
 *          the multiset of run-sums until you reach a squarefree number. This
 *          number is prime (or 1) iff n belongs to the sequence.
 * https://oeis.org/A353844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer that belongs to * the set of numbers
 *            whose multiset of prime indices, * after repeated run‑sum
 *            transformation, reaches an empty * set or a singleton. The
 *            sequence is defined by the * O.E.I.S. entry A353844. This
 *            implementation searches * for terms on demand and memoises
 *            the...
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ---------- prime generation (static) ---------- */
#define PRIME_LIMIT 2000000ULL

static bool primes_generated = false;
static uint32_t *prime_list = NULL;       /* list of primes */
static size_t prime_count = 0;            /* number of primes */
static uint32_t *prime_index = NULL;      /* prime_index[p] = 1‑based index of prime p, 0 if not prime */

/* comparison for qsort of uint64_t */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

/* build prime table once */
static void init_primes(void)
{
    if (primes_generated) return;
    /* sieve */
    bool *is_prime = (bool *)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    prime_index = (uint32_t *)malloc((PRIME_LIMIT + 1) * sizeof(uint32_t));
    if (!is_prime || !prime_index) exit(1);
    for (uint32_t i = 0; i <= PRIME_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (uint32_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = false;
    /* count primes and build list */
    prime_count = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) ++prime_count;
    prime_list = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    if (!prime_list) exit(1);
    size_t idx = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            prime_list[idx++] = i;
            prime_index[i] = (uint32_t)(idx); /* 1‑based */
        } else {
            prime_index[i] = 0;
        }
    }
    primes_generated = true;
    free(is_prime);
}

/* ---------------- membership test ---------------- */
static bool in_sequence(uint64_t n)
{
    init_primes();

    /* arrays for current multiset and next multiset */
    static const size_t MAXV = 4096; /* generous upper bound */
    uint64_t cur[MAXV], nxt[MAXV], prev[MAXV];
    size_t cur_sz = 0, nxt_sz = 0, prev_sz = 0;

    /* factorise n: gather prime indices with multiplicity */
    uint64_t rem = n;
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= rem; ++i) {
        uint32_t p = prime_list[i];
        while (rem % p == 0) {
            cur[cur_sz++] = (uint64_t)prime_index[p];
            rem /= p;
        }
    }
    if (rem > 1) { /* remaining prime factor > sqrt(n) */
        /* ensure it's within PRIME_LIMIT */
        if (rem > PRIME_LIMIT) return false; /* safety: not handled */
        cur[cur_sz++] = (uint64_t)prime_index[(uint32_t)rem];
    }
    if (cur_sz == 0) return true; /* n = 1 */

    /* repeated run‑sum transformation */
    while (1) {
        qsort(cur, cur_sz, sizeof(uint64_t), cmp_uint64);
        if (cur_sz <= 1) return true;                 /* empty or singleton reached */
        if (prev_sz == cur_sz && memcmp(cur, prev, cur_sz * sizeof(uint64_t)) == 0)
            return false;                            /* loop detected */
        memcpy(prev, cur, cur_sz * sizeof(uint64_t));
        prev_sz = cur_sz;

        /* compute next multiset */
        size_t i = 0;
        nxt_sz = 0;
        while (i < cur_sz) {
            uint64_t val = cur[i];
            size_t cnt = 1;
            ++i;
            while (i < cur_sz && cur[i] == val) { ++cnt; ++i; }
            nxt[nxt_sz++] = val * (uint64_t)cnt;
        }
        memcpy(cur, nxt, nxt_sz * sizeof(uint64_t));
        cur_sz = nxt_sz;
    }
}

/* ----------------- public function ----------------- */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t scanner = 1; /* next integer to test */

    while (cache_len < n) {
        if (in_sequence(scanner)) {
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap > 0) ? cache_cap * 2 : 8;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) exit(1);
            }
            cache[cache_len++] = (seq_t)scanner;
        }
        ++scanner;
    }
    return cache[n - 1];
}
