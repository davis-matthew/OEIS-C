/*
 * A334433  Heinz numbers of all integer partitions sorted first by sum, then
 *          by length, and finally lexicographically.
 * https://oeis.org/A334433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = Heinz number of the partition corresponding to n in
 *            Abramowitz-Stegun ordering, sorted by sum, then length, then
 *            lexicographically
 * Range:     u8 n <= 65, u16 n <= 913, u32 n <= 2713, u64 n <= 2713,
 *            u128 n <= 2713, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* ---- Partition representation ---- */
typedef struct {
    idx_t *parts;   /* array of parts, in weakly decreasing order */
    size_t len;     /* number of parts */
} Partition;

/* ---- Global state ---- */
static seq_t *prime_vals = NULL; /* 1‑indexed list of primes (prime_vals[1]=2) */
static size_t prime_cnt   = 0;

static seq_t *cache  = NULL; /* cached sequence terms */
static size_t cache_len = 0; /* number of cached terms */
static size_t cache_cap = 0; /* allocated capacity of cache */

static idx_t next_sum = 0;    /* next sum to generate */
static bool initialized = false;

/* ---- Ensure cache has space for at least one more element ---- */
static void ensure_cache_cap(void) {
    if (cache_len < cache_cap) return;
    cache_cap = (cache_cap == 0) ? 256 : (cache_cap << 1);
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
}

/* ---- Generate enough primes up to a given limit ---- */
static void generate_primes_up_to(size_t limit) {
    if (prime_cnt != 0) return;           /* already built */

    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return; /* out of memory */

    memset(is_prime, 1, limit + 1);
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    prime_cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++prime_cnt;

    prime_vals = (seq_t *)malloc((prime_cnt + 1) * sizeof(seq_t));
    prime_vals[0] = 0; /* unused */
    size_t idx = 1;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) prime_vals[idx++] = (seq_t)i;

    free(is_prime);
}

/* ---- Recursive generation of all weakly decreasing partitions of 'rem' ---- */
static void gen_partitions(idx_t rem, idx_t max_part, idx_t depth,
                           idx_t cur[],
                           Partition **arr, size_t *sz, size_t *cap) {
    if (rem == 0) {
        Partition p;
        p.len   = depth;
        p.parts = (idx_t *)malloc(depth * sizeof(idx_t));
        memcpy(p.parts, cur, depth * sizeof(idx_t));

        if (*sz >= *cap) {
            *cap = (*cap == 0) ? 16 : (*cap << 1);
            *arr = (Partition *)realloc(*arr, *cap * sizeof(Partition));
        }
        (*arr)[*sz] = p;
        ++(*sz);
        return;
    }

    for (idx_t part = rem < max_part ? rem : max_part; part >= 1; --part) {
        cur[depth] = part;
        gen_partitions(rem - part, part, depth + 1,
                       cur, arr, sz, cap);
    }
}

/* ---- Comparison function for qsort: length asc, then lex asc ---- */
static int cmp_partition(const void *a, const void *b) {
    const Partition *pa = (const Partition *)a;
    const Partition *pb = (const Partition *)b;

    if (pa->len != pb->len)
        return (pa->len < pb->len) ? -1 : 1;

    for (size_t i = 0; i < pa->len; ++i) {
        if (pa->parts[i] != pb->parts[i])
            return (pa->parts[i] < pb->parts[i]) ? -1 : 1;
    }
    return 0;
}

/* ---- Append a new term to the cache ---- */
static void append_cache(seq_t val) {
    ensure_cache_cap();
    cache[cache_len++] = val;
}

/* ---- Generate all partitions of the next_sum, ordered as required,
       compute their Heinz numbers, and add them to the cache. ---- */
static void generate_next_sum(void) {
    if (next_sum == 0) {
        /* Empty partition -> value 1 */
        append_cache((seq_t)1);
        ++next_sum;
        return;
    }

    idx_t cur[64];                 /* temporary buffer for a partition */
    Partition *parts = NULL;
    size_t parts_sz = 0, parts_cap = 0;

    gen_partitions(next_sum, next_sum, 0, cur,
                   &parts, &parts_sz, &parts_cap);

    /* Sort by length then lexicographically */
    qsort(parts, parts_sz, sizeof(Partition), cmp_partition);

    /* Compute Heinz numbers in sorted order and cache them */
    for (size_t i = 0; i < parts_sz; ++i) {
        seq_t prod = (seq_t)1;
        for (size_t j = 0; j < parts[i].len; ++j) {
            idx_t part = parts[i].parts[j];
            prod *= prime_vals[part];   /* prime(part) */
        }
        append_cache(prod);
    }

    /* Clean up temporary structures */
    for (size_t i = 0; i < parts_sz; ++i)
        free(parts[i].parts);
    free(parts);

    ++next_sum;
}

/* ---- Ensure global state is initialized lazily ---- */
static void ensure_initialized(void) {
    if (initialized) return;
    /* Generate enough primes for any reasonable sum. 2000 covers the needed range. */
    generate_primes_up_to(2000);
    cache  = NULL;
    cache_cap = 0;
    cache_len = 0;
    next_sum = 0;
    initialized = true;
}

/* ---- Public sequence function ---- */
seq_t a(idx_t n) {
    ensure_initialized();

    /* Generate terms until we have at least n+1 cached entries */
    while (cache_len <= n) {
        generate_next_sum();
    }

    return cache[n];
}
