/*
 * A280027  Fill an infinite square array by following a spiral around the
 *          origin; in the central cell enter a(0)=1; thereafter, in the n-th
 *          cell, enter the sum of the entries of those earlier cells that can
 *          be seen from that cell.
 * https://oeis.org/A280027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(0) = 1 For n > 0: a(n) = sum of all previously computed a(k)
 *            that lie on the same row, column, diagonal, or antidiagonal as
 *            the cell at position n in the square spiral starting at the
 *            origin
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 37, u64 n <= 71, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required includes for the C11 / C++17 build. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

/* Helper hash table for mapping line identifiers to cumulative sums. */
typedef struct {
    size_t capacity;      /* must be a power of two */
    size_t size;          /* number of used entries */
    int64_t *key;         /* line identifier */
    seq_t   *value;       /* cumulative sum for that line */
} ht_t;

/* Sentinel value for an empty key slot.  INT64_MIN is not used as a valid key. */
#define HT_SENTINEL INT64_MIN

/* Compute a hash index; assumes capacity is a power of two. */
static size_t ht_hash(const ht_t *ht, int64_t k)
{
    uint64_t u = (uint64_t)k;
    /* xorshift to mix bits.  Effectively: u ^ (u >> 33) */
    return (size_t)(u ^ (u >> 33)) & (ht->capacity - 1);
}

/* Allocate a new hash table with the given initial capacity (power of two). */
static void ht_init(ht_t *ht, size_t init_capacity)
{
    ht->capacity = init_capacity;
    ht->size = 0;
    ht->key   = (int64_t *)malloc(ht->capacity * sizeof(int64_t));
    ht->value = (seq_t  *)calloc(ht->capacity, sizeof(seq_t)); /* zero init */
    for (size_t i = 0; i < ht->capacity; ++i) ht->key[i] = HT_SENTINEL;
}

/* Rehash the table to double its capacity. */
static void ht_rehash(ht_t *ht)
{
    size_t new_capacity = ht->capacity << 1;
    int64_t *new_key = (int64_t *)malloc(new_capacity * sizeof(int64_t));
    seq_t   *new_value = (seq_t  *)calloc(new_capacity, sizeof(seq_t));
    for (size_t i = 0; i < new_capacity; ++i) new_key[i] = HT_SENTINEL;

    size_t old_capacity = ht->capacity;
    for (size_t i = 0; i < old_capacity; ++i) {
        if (ht->key[i] != HT_SENTINEL) {
            int64_t k = ht->key[i];
            seq_t v = ht->value[i];
            size_t pos = ht_hash(ht, k);
            while (new_key[pos] != HT_SENTINEL) pos = (pos + 1) & (new_capacity - 1);
            new_key[pos] = k;
            new_value[pos] = v;
        }
    }

    free(ht->key);
    free(ht->value);
    ht->key = new_key;
    ht->value = new_value;
    ht->capacity = new_capacity;
}

/* Return the sum associated with key, or zero if key not present. */
static seq_t ht_get(const ht_t *ht, int64_t k)
{
    size_t pos = ht_hash(ht, k);
    while (true) {
        if (ht->key[pos] == HT_SENTINEL) return (seq_t)0;
        if (ht->key[pos] == k) return ht->value[pos];
        pos = (pos + 1) & (ht->capacity - 1);
    }
}

/* Add val to the cumulative sum for key (inserting if necessary). */
static void ht_add(ht_t *ht, int64_t k, seq_t val)
{
    size_t pos = ht_hash(ht, k);
    while (true) {
        if (ht->key[pos] == HT_SENTINEL) {
            /* Insert new key. */
            ht->key[pos] = k;
            ht->value[pos] = val;
            ++ht->size;
            /* Resize if load factor > 0.75 to keep operations O(1). */
            if (ht->size * 4 > ht->capacity * 3) ht_rehash(ht);
            return;
        }
        if (ht->key[pos] == k) {
            /* Existing key. */
            ht->value[pos] += val;
            return;
        }
        pos = (pos + 1) & (ht->capacity - 1);
    }
}

/* --------------------------  Memoized spiral generator  -------------------------- */

static bool initialized = false;
static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static idx_t  cache_size = 0;       /* number of cached values (current max index + 1) */
static idx_t  next_index = 1;       /* next index to compute */

static ht_t row_ht, col_ht, diag_ht, anti_ht;   /* hash tables for line sums */

static void ensure_cache_capacity(idx_t needed)
{
    while (cache_capacity <= (size_t)needed) {
        size_t new_cap = cache_capacity ? cache_capacity << 1 : 16;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();  /* out of memory */
        cache = new_cache;
        cache_capacity = new_cap;
    }
}

static void init_memoization(void)
{
    /* Allocate cache for a(0). */
    cache_capacity = 16;
    cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
    cache[0] = (seq_t)1;        /* a(0) = 1 */
    cache_size = 1;
    next_index = 1;             /* next to compute */

    /* Initialize hash tables with capacity 16. */
    ht_init(&row_ht, 16);
    ht_init(&col_ht, 16);
    ht_init(&diag_ht, 16);
    ht_init(&anti_ht, 16);

    /* Record the central cell in all four tables. */
    ht_add(&row_ht,  0, cache[0]);  /* row 0 */
    ht_add(&col_ht,  0, cache[0]);  /* column 0 */
    ht_add(&diag_ht, 0, cache[0]);  /* diagonal 0 (x - y) */
    ht_add(&anti_ht, 0, cache[0]);  /* antidiagonal 0 (x + y) */

    initialized = true;
}

/* Generate spiral coordinates and compute values up to n (inclusive). */
static void compute_to(idx_t n)
{
    if (!initialized) init_memoization();

    /* Position state variables for continuation from last computed cell. */
    static int64_t cur_x = 0, cur_y = 0;
    static int dir = 0;            /* 0:right, 1:up, 2:left, 3:down */
    static int64_t step_len = 1;   /* number of steps in current leg */
    static int steps_remaining = 1; /* steps left in current leg */

    /* We compute from next_index up to n inclusive. */
    for (idx_t i = next_index; i <= n; ++i) {
        /* Move one step in the current direction. */
        switch (dir) {
            case 0: ++cur_x; break;   /* right  */
            case 1: ++cur_y; break;   /* up     */
            case 2: --cur_x; break;   /* left   */
            case 3: --cur_y; break;   /* down   */
        }
        --steps_remaining;
        if (steps_remaining == 0) {
            dir = (dir + 1) & 3;
            if ((dir & 1) == 0) ++step_len;   /* increase step after left (dir == 2) or right (dir == 0) */
            steps_remaining = (int)step_len;
        }

        /* Sum contributions from all lines that intersect this cell. */
        seq_t val = (seq_t)0;
        val += ht_get(&row_ht,  cur_x);
        val += ht_get(&col_ht,  cur_y);
        val += ht_get(&diag_ht, cur_x - cur_y);
        val += ht_get(&anti_ht, cur_x + cur_y);

        /* Store the computed value. */
        ensure_cache_capacity(i);
        cache[(size_t)i] = val;
        ++cache_size;

        /* Update line sums with this new value. */
        ht_add(&row_ht,  cur_x, val);
        ht_add(&col_ht,  cur_y, val);
        ht_add(&diag_ht, cur_x - cur_y, val);
        ht_add(&anti_ht, cur_x + cur_y, val);
    }

    next_index = n + 1;   /* next index to compute */
}

/* Public interface as requested. */
seq_t a(idx_t n)
{
    compute_to(n);
    return cache[(size_t)n];
}
