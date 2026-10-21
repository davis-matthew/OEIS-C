/*
 * A269969  Index of first occurrence of -n in A269783.
 * https://oeis.org/A269969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 686, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Index of first occurrence of −n in A269783
    (OEIS sequence A269969)

    The function generates A269783 on demand, keeping
    counts of each signed value and recording the first
    index at which −n appears.  It works for any
    “seq_t” and “idx_t” supplied by the build.
    For seq_t that is unsigned it keeps the sign
    separately so that negative numbers can be handled.
*/

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* malloc, calloc, realloc */
#include <stdbool.h>  /* bool */

typedef seq_t seq_t;
typedef idx_t idx_t;

/* ---------- Signed value representation ---------- */
typedef struct { bool neg; seq_t abs; } signed_val_t;

/* ---------- Hash table for counts of signed values ---------- */
typedef struct {
    signed_val_t key;
    uint32_t count;
    bool used;
} ht_entry_t;

static ht_entry_t *ht_tbl     = NULL;
static size_t      ht_cap     = 0;   /* power of two */
static size_t      ht_mask    = 0;

static void ht_init(void)
{
    if (ht_tbl) return;
    ht_cap  = 1U << 20;             /* 1,048,576 entries – ample for n≈10 000 */
    ht_mask = ht_cap - 1;
    ht_tbl = (ht_entry_t *)calloc(ht_cap, sizeof(ht_entry_t));
}

/* Simple hash based on lower 64‑bits of abs value plus sign */
static inline size_t ht_hash(const signed_val_t *v)
{
    uint64_t h = (uint64_t) v->abs;
    h = h * 11400714819323198485ull;            /* Fibonacci multiplier */
    h ^= (uint64_t)(v->neg ? 1234567ULL : 7654321ULL);
    return (size_t)(h) & ht_mask;
}

static ht_entry_t *ht_find_or_create(const signed_val_t *k)
{
    size_t idx = ht_hash(k);
    while (ht_tbl[idx].used) {
        if (!ht_tbl[idx].key.neg && !k->neg &&
            ht_tbl[idx].key.abs == k->abs) {
            return &ht_tbl[idx];
        }
        if (ht_tbl[idx].key.neg == k->neg &&
            ht_tbl[idx].key.abs == k->abs) {
            return &ht_tbl[idx];
        }
        idx = (idx + 1) & ht_mask;
    }
    ht_tbl[idx].used  = true;
    ht_tbl[idx].key   = *k;
    ht_tbl[idx].count = 0;
    return &ht_tbl[idx];
}

static inline void ht_inc(const signed_val_t *k)
{
    ht_find_or_create(k)->count++;
}

static inline uint32_t ht_get_count(const signed_val_t *k)
{
    size_t idx = ht_hash(k);
    while (ht_tbl[idx].used) {
        if (!ht_tbl[idx].key.neg && !k->neg &&
            ht_tbl[idx].key.abs == k->abs) {
            return ht_tbl[idx].count;
        }
        if (ht_tbl[idx].key.neg == k->neg &&
            ht_tbl[idx].key.abs == k->abs) {
            return ht_tbl[idx].count;
        }
        idx = (idx + 1) & ht_mask;
    }
    return 0;
}

/* ---------- Storage for first‑occurrence indices of −n ---------- */
static idx_t *first_occ = NULL;   /* first_occ[m] = index of −m (0 if not yet seen) */
static size_t first_cap = 0;

static void ensure_first_occ_cap(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (first_cap >= needed) return;
    size_t new_cap = first_cap ? first_cap * 2 : 256;
    while (new_cap < needed) new_cap <<= 1;
    first_occ = (idx_t *)realloc(first_occ, new_cap * sizeof(idx_t));
    /* newly allocated memory is zeroed by realloc on calloc‑like alloc */
    first_cap = new_cap;
}

/* ---------- State for generating A269783 ---------- */
static bool init_done = false;
static signed_val_t current_val;      /* current A269783 value */
static idx_t        current_idx = 0;  /* index of current_val */

static void init_core(void)
{
    if (init_done) return;
    init_done = true;

    ht_init();

    /* initial value A269783(0) = 0 */
    current_val.neg = false;
    current_val.abs = 0;
    current_idx = 0;
    ht_inc(&current_val);

    ensure_first_occ_cap(0);
    first_occ[0] = 0;
}

/* ---------- Generate next term of A269783 ---------- */
static void generate_next(void)
{
    /* k = number of occurrences of current_val so far (including this one) */
    uint32_t k = ht_get_count(&current_val);

    /* e = 1 if current_val > 0, else 0 */
    bool is_pos = !current_val.neg && current_val.abs != (seq_t)0;
    int   e = is_pos ? 1 : 0;

    /* floor(k/2) as seq_t */
    seq_t half = (seq_t)(k >> 1);

    /* sign = (-1)^(k+e) */
    int  sign_factor = ((k + e) & 1) ? -1 : 1;

    /* compute new value by applying delta = sign_factor * half to current_val */
    signed_val_t new_val;
    if (sign_factor == 1) {                 /* add half */
        if (!current_val.neg) {              /* current non‑negative */
            new_val.neg = false;
            new_val.abs = current_val.abs + half;
        } else {                             /* current negative */
            if (current_val.abs > half) {     /* magnitude stays negative */
                new_val.neg = true;
                new_val.abs = current_val.abs - half;
            } else if (current_val.abs == half) { /* becomes zero */
                new_val.neg = false;
                new_val.abs = 0;
            } else { /* |current| < half -> becomes positive */
                new_val.neg = false;
                new_val.abs = half - current_val.abs;
            }
        }
    } else {                                  /* subtract half */
        if (!current_val.neg) {              /* current non‑negative */
            if (current_val.abs > half) {     /* stays non‑negative */
                new_val.neg = false;
                new_val.abs = current_val.abs - half;
            } else if (current_val.abs == half) {
                new_val.neg = false;
                new_val.abs = 0;
            } else { /* becomes negative */
                new_val.neg = true;
                new_val.abs = half - current_val.abs;
            }
        } else {                             /* current negative */
            new_val.neg = true;
            new_val.abs = current_val.abs + half;   /* magnitude increases */
        }
    }

    /* update count for new_val */
    ht_inc(&new_val);

    /* record first occurrence of −magnitude if seen for the first time */
    if (new_val.neg) {
        idx_t mag = (idx_t)new_val.abs;    /* magnitude fits into idx_t */
        ensure_first_occ_cap(mag);
        if (first_occ[mag] == 0)            /* not seen before */
            first_occ[mag] = current_idx + 1; /* index of new element */
    }

    /* advance state */
    current_val = new_val;
    current_idx++;
}

/* ---------- Public interface ---------- */
seq_t a(idx_t n)
{
    init_core();

    if (n == 0) return 0;          /* a(0) = 0 by definition */

    ensure_first_occ_cap(n);

    if (first_occ[n] != 0)          /* already known */
        return (seq_t)first_occ[n];

    /* generate values until −n occurs */
    while (first_occ[n] == 0)
        generate_next();

    return (seq_t)first_occ[n];
}
