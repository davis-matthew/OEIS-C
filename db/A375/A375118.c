/*
 * A375118  Let {b(m)} be Recamán's sequence A005132, with the additional term
 *          b(-1):=0. Define a(n) to be the first index m where b(m-1)-m = -n,
 *          or -1 if b(m-1)-m never equals -n.
 * https://oeis.org/A375118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A375118: a(n) is the first index m such that
 * Recamán’s sequence b satisfies b(m-1) – m = –n.
 * This implementation simulates the Recamán sequence and
 * records when the required difference occurs. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* -------------------------------------------------- */
/*  Recamán state                                      */
static idx_t current_m; /* last processed index (m-1) */
static seq_t current_b; /* b(m-1)                          */

/* -------------------------------------------------- */
/*  Visited set : open-addressing hash table          */
typedef struct {
    size_t cap;          /* capacity (power of two)        */
    size_t mask;         /* cap-1                           */
    size_t used;         /* number of elements              */
    uint64_t *keys;      /* keys (truncated to 64‑bit)     */
    unsigned char *flags;/* 0 = empty, 1 = used             */
} VisitedSet;

static VisitedSet visited = {0, 0, 0, NULL, NULL};

static void visited_init(void)
{
    visited.cap   = 16;
    visited.mask  = visited.cap - 1;
    visited.used  = 0;
    visited.keys  = (uint64_t *)malloc(visited.cap * sizeof(uint64_t));
    visited.flags = (unsigned char *)calloc(visited.cap, sizeof(unsigned char));

    /* b(0) = 0 is the first Recamán number */
    visited.keys[0]  = 0;
    visited.flags[0] = 1;
    visited.used = 1;
}

static void visited_resize(void)
{
    size_t new_cap   = visited.cap << 1;
    size_t new_mask  = new_cap - 1;
    uint64_t *new_keys  = (uint64_t *)malloc(new_cap * sizeof(uint64_t));
    unsigned char *new_flags =
        (unsigned char *)calloc(new_cap, sizeof(unsigned char));

    for (size_t i = 0; i < visited.cap; ++i) {
        if (visited.flags[i]) {
            uint64_t k = visited.keys[i];
            size_t  j = k & new_mask;
            while (new_flags[j]) j = (j + 1) & new_mask;
            new_keys[j]  = k;
            new_flags[j] = 1;
        }
    }
    free(visited.keys);
    free(visited.flags);
    visited.keys  = new_keys;
    visited.flags = new_flags;
    visited.cap   = new_cap;
    visited.mask  = new_mask;
}

static void visited_insert(uint64_t key)
{
    if (visited.used * 4 >= visited.cap * 3) visited_resize();

    size_t idx = key & visited.mask;
    while (visited.flags[idx]) {
        if (visited.keys[idx] == key) return;
        idx = (idx + 1) & visited.mask;
    }
    visited.keys[idx]  = key;
    visited.flags[idx] = 1;
    visited.used++;
}

static bool visited_contains(uint64_t key)
{
    size_t idx = key & visited.mask;
    while (visited.flags[idx]) {
        if (visited.keys[idx] == key) return true;
        idx = (idx + 1) & visited.mask;
    }
    return false;
}

/* -------------------------------------------------- */
/*  Map: n → first m that achieves –n                */
typedef struct {
    size_t cap;   /* array size (power of two)          */
    idx_t *data;  /* contains m or (idx_t)-1 for unknown  */
    idx_t unk;    /* value used to indicate unknown    */
} Map;

static Map map = {0, NULL, ~(idx_t)0};

static void map_init(void)
{
    map.cap   = 1024;
    map.data  = (idx_t *)malloc(map.cap * sizeof(idx_t));
    for (size_t i = 0; i < map.cap; ++i) map.data[i] = map.unk;
    map.data[0] = 0; /* a(0) = 0 */
}

static void map_resize(size_t new_cap)
{
    idx_t *new_data = (idx_t *)malloc(new_cap * sizeof(idx_t));
    for (size_t i = 0; i < new_cap; ++i) new_data[i] = map.unk;
    for (size_t i = 0; i < map.cap && i < new_cap; ++i) new_data[i] = map.data[i];
    free(map.data);
    map.data = new_data;
    map.cap  = new_cap;
}

static void map_set(idx_t key, idx_t val)
{
    if (key >= map.cap) {
        size_t new_cap = map.cap;
        while (key >= new_cap) new_cap <<= 1;
        map_resize(new_cap);
    }
    map.data[key] = val;
}

static idx_t map_get(idx_t key)
{
    if (key >= map.cap) return map.unk;
    return map.data[key];
}

/* -------------------------------------------------- */
/*  Initialization                                      */
static bool initialized = false;

static void init(void)
{
    if (initialized) return;
    visited_init();
    map_init();
    current_m = 0;
    current_b = 0;
    initialized = true;
}

/* -------------------------------------------------- */
/*  Generate Recamán values until n has an entry.     */
static void generate_until(idx_t target_n)
{
    /* Ensure the map can hold target_n (and any other n encountered) */
    if (target_n >= map.cap) {
        size_t new_cap = map.cap;
        while (target_n >= new_cap) new_cap <<= 1;
        map_resize(new_cap);
    }

    /* Loop until we have found a value for target_n */
    while (map_get(target_n) == map.unk) {
        idx_t m = current_m + 1;     /* next index to compute           */
        __int128 diff128 = (__int128)current_b - (__int128)m;

        /* Record when the difference equals –n */
        if (diff128 < 0) {
            idx_t n = (idx_t)(-diff128);
            if (map_get(n) == map.unk) map_set(n, m);
        }

        /* Choose the next Recamán value */
        seq_t next_b;
        if (diff128 >= 0 && !visited_contains((uint64_t)diff128)) {
            next_b = (seq_t)diff128;
        } else {
            next_b = (seq_t)((__int128)current_b + (__int128)m);
        }

        visited_insert((uint64_t)next_b);
        current_b = next_b;
        current_m = m;
    }
}

/* -------------------------------------------------- */
/*  Main sequence function                              */
seq_t a(idx_t n)
{
    init();
    if (n >= map.cap || map_get(n) == map.unk) {
        generate_until(n);
    }
    idx_t m = map_get(n);
    if (m == map.unk) return (seq_t)(-1);
    return (seq_t)m;
}
