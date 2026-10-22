/*
 * A309390  Set a(1)=10. Thereafter a(n) is the smallest positive number not
 *          yet in the sequence that contains exactly one even digit and one
 *          odd digit from a(n-1).
 * https://oeis.org/A309390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   digit_masks
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(1)=10; for n>1, a(n) is the smallest positive integer not
 * yet in the sequence that contains at least one even digit from a(n-1)
 * and at least one odd digit from a(n-1). */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_masks.h"

/* Hash table for membership testing */
#define HASH_INIT_CAP 32
static seq_t *hash_keys = NULL;
static unsigned char *hash_used = NULL;
static size_t hash_size = 0;
static size_t hash_count = 0;

static void hash_resize(void)
{
    size_t new_size = hash_size * 2;
    seq_t *new_keys = (seq_t*)malloc(new_size * sizeof(seq_t));
    unsigned char *new_used = (unsigned char*)malloc(new_size);
    for (size_t i = 0; i < new_size; ++i) { new_used[i] = 0; }

    /* rehash existing keys */
    for (size_t i = 0; i < hash_size; ++i) {
        if (hash_used[i]) {
            seq_t key = hash_keys[i];
            size_t h = (size_t)key;
            h ^= h >> 33;
            h *= 0xff51afd7ed558ccdULL;
            h ^= h >> 33;
            h *= 0xc4ceb9fe1a85ec53ULL;
            h ^= h >> 33;
            size_t pos = h & (new_size - 1);
            while (new_used[pos]) pos = (pos + 1) & (new_size - 1);
            new_keys[pos] = key;
            new_used[pos] = 1;
        }
    }
    free(hash_keys); free(hash_used);
    hash_keys = new_keys;
    hash_used = new_used;
    hash_size = new_size;
}

static void hash_init(void)
{
    hash_size = HASH_INIT_CAP;
    hash_keys = (seq_t*)malloc(hash_size * sizeof(seq_t));
    hash_used = (unsigned char*)malloc(hash_size);
    for (size_t i = 0; i < hash_size; ++i) hash_used[i] = 0;
    hash_count = 0;
}

static bool hash_contains(seq_t v)
{
    if (!hash_keys) hash_init();
    size_t h = (size_t)v;
    h ^= h >> 33;
    h *= 0xff51afd7ed558ccdULL;
    h ^= h >> 33;
    h *= 0xc4ceb9fe1a85ec53ULL;
    h ^= h >> 33;
    size_t pos = h & (hash_size - 1);
    while (hash_used[pos]) {
        if (hash_keys[pos] == v) return true;
        pos = (pos + 1) & (hash_size - 1);
    }
    return false;
}

static void hash_insert(seq_t v)
{
    if (!hash_keys) hash_init();
    if ((hash_count + 1) * 10 >= hash_size * 7) hash_resize();
    size_t h = (size_t)v;
    h ^= h >> 33;
    h *= 0xff51afd7ed558ccdULL;
    h ^= h >> 33;
    h *= 0xc4ceb9fe1a85ec53ULL;
    h ^= h >> 33;
    size_t pos = h & (hash_size - 1);
    while (hash_used[pos]) {
        if (hash_keys[pos] == v) return;    /* already present */
        pos = (pos + 1) & (hash_size - 1);
    }
    hash_keys[pos] = v;
    hash_used[pos] = 1;
    ++hash_count;
}

/* Sequence cache */
static seq_t *seq_cache = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;   /* number of terms stored, index 1..seq_len */

static void seq_resize(size_t min_needed)
{
    if (seq_cap >= min_needed) return;
    size_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap < min_needed) new_cap *= 2;
    seq_cache = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Test whether candidate contains at least one digit from each mask */
static bool candidate_ok(seq_t cand, const bool evens[10], const bool odds[10])
{
    bool seen_even = 0, seen_odd = 0;
    if (cand == 0) return 0; /* never used */
    while (cand > 0) {
        int d = (int)(cand % 10);
        cand /= 10;
        if ((d & 1) == 0) {
            if (evens[d]) seen_even = 1;
        } else {
            if (odds[d]) seen_odd = 1;
        }
        if (seen_even && seen_odd) return 1;
    }
    return 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset 1; caller may ignore n==0 */
    if (seq_len == 0) { /* first time called: init cache */
        seq_resize(n + 2);
        seq_cache[1] = (seq_t)10;
        hash_insert((seq_t)10);
        seq_len = 1;
    }
    if ((size_t)n > seq_len) {
        seq_resize((size_t)n + 2);
        for (size_t i = seq_len + 1; i <= (size_t)n; ++i) {
            seq_t prev = seq_cache[i - 1];
            bool evens[10], odds[10];
            digit_masks(prev, evens, odds);
            seq_t cand = 1;
            while (1) {
                if (!hash_contains(cand) && candidate_ok(cand, evens, odds)) {
                    seq_cache[i] = cand;
                    hash_insert(cand);
                    break;
                }
                ++cand;
            }
        }
        seq_len = (size_t)n;
    }
    return seq_cache[(size_t)n];
}
