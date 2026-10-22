/*
 * A364835  a(1) = 1; a(n+1) = (number of times a(n)+1 has appeared) - (number
 *          of times a(n) has appeared).
 * https://oeis.org/A364835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, look
 * Formula:   a(1) = 1; for n ≥ 1, a(n+1) = (# of times a(n)+1 has appeared) -
 *            (# of times a(n) has appeared). For n ≥ 149695: if n mod 3 == 0:
 *            a(n) = 49456 – n/3 else : a(n) = (n mod 3) – 1
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* hash table entry for counts of seq_t values */
typedef struct {
    seq_t    key;
    uint32_t count;
    uint8_t  used;
} HashEntry;

static HashEntry *hash_table = NULL;
static size_t hash_size  = 0;     /* table size, power of two */
static size_t hash_mask  = 0;
static size_t hash_used  = 0;     /* number of used slots */
#define HASH_LOAD_FACTOR 0x7F   /* 0.7 in 8‑bit fixed point = 0.7 */

/* Fowler–Noll–Vo‑like hash for seq_t (works for built‑in and big ints) */
static inline uint64_t hash_seq_t(seq_t x)
{
#if defined(__SIZEOF_INT128__)
    unsigned __int128 ux = (unsigned __int128)x;
    uint64_t h = (uint64_t)ux ^ (uint64_t)(ux >> 64);
#else
    uint64_t h = (uint64_t)(uintptr_t)x;
#endif
    h ^= h >> 33;
    h *= 0xff51afd7ed558ccdULL;
    h ^= h >> 33;
    h *= 0xc4ceb9fe1a85ec53ULL;
    h ^= h >> 33;
    return h;
}

/* resize the hash table when load factor exceeded */
static void hash_resize(void)
{
    size_t new_size = hash_size << 1;
    HashEntry *new_table = (HashEntry *)calloc(new_size, sizeof(HashEntry));
    size_t new_mask = new_size - 1;

    for (size_t i = 0; i < hash_size; ++i) {
        if (hash_table[i].used) {
            seq_t   k = hash_table[i].key;
            uint32_t c = hash_table[i].count;
            size_t h = hash_seq_t(k) & new_mask;
            while (new_table[h].used) h = (h + 1) & new_mask;
            new_table[h].used  = 1;
            new_table[h].key   = k;
            new_table[h].count = c;
        }
    }

    free(hash_table);
    hash_table = new_table;
    hash_size  = new_size;
    hash_mask  = new_mask;
}

/* ensure hash table has capacity for one more element */
static void hash_ensure_capacity(void)
{
    if (hash_used + 1 > (size_t)((uint64_t)hash_size * HASH_LOAD_FACTOR >> 8))
        hash_resize();
}

/* initialize hash table once */
static void hash_init(void)
{
    hash_size = 1 << 10;   /* 1024, power of two */
    hash_mask = hash_size - 1;
    hash_used = 0;
    hash_table = (HashEntry *)calloc(hash_size, sizeof(HashEntry));
}

/* increment count of key in hash table */
static void hash_increment(seq_t key)
{
    hash_ensure_capacity();
    size_t h = hash_seq_t(key) & hash_mask;
    while (hash_table[h].used) {
        if (hash_table[h].key == key) {
            ++hash_table[h].count;
            return;
        }
        h = (h + 1) & hash_mask;
    }
    hash_table[h].used  = 1;
    hash_table[h].key   = key;
    hash_table[h].count = 1;
    ++hash_used;
}

/* get count of key, 0 if not present */
static uint32_t hash_get_count(seq_t key)
{
    size_t h = hash_seq_t(key) & hash_mask;
    while (hash_table[h].used) {
        if (hash_table[h].key == key)
            return hash_table[h].count;
        h = (h + 1) & hash_mask;
    }
    return 0;
}

/* dynamic array for sequence values: 0‑based, seq_arr[0] = a(1) */
static seq_t *seq_arr = NULL;
static size_t seq_cap = 0;  /* allocated slots */
static size_t seq_len = 0;  /* number of values stored */

/* grow array if needed */
static void seq_grow_if_needed(void)
{
    if (seq_len >= seq_cap) {
        seq_cap = (seq_cap == 0) ? 128 : seq_cap << 1;
        seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
    }
}

/* --------------------------------------------------------------------- */
/* Public function: compute a(n) for any n >= 1, independently of call order */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined, return 0 safely */
        return (seq_t)0;

    /* One‑time initialization */
    static int init_done = 0;
    if (!init_done) {
        seq_cap = 128;
        seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_arr[0] = (seq_t)1;   /* a(1) = 1 */
        seq_len   = 1;

        hash_init();
        hash_increment((seq_t)1);

        init_done = 1;
    }

    /* Return cached value if already computed */
    if ((size_t)n <= seq_len)
        return seq_arr[n - 1];

    /* For large n use closed form directly */
    if (n > 149695) {
        if ((n % 3ULL) == 0ULL) {
            return (seq_t)49456 - (seq_t)(n / 3);
        } else {
            int rem = (int)(n % 3ULL);
            return (seq_t)(rem - 1);
        }
    }

    /* Compute terms up to n (n <= 149695) */
    for (size_t i = seq_len + 1; i <= n; ++i) {
        seq_t prev = seq_arr[i - 2];                     /* a(i-1) */
        uint32_t cnt_plus1 = hash_get_count(prev + (seq_t)1);
        uint32_t cnt_prev  = hash_get_count(prev);
        seq_t new_val = (seq_t)cnt_plus1 - (seq_t)cnt_prev;

        seq_grow_if_needed();
        seq_arr[seq_len++] = new_val;
        hash_increment(new_val);
    }

    return seq_arr[n - 1];
}
