/*
 * A394421  a(1) = 1, a(2) = 2; for n > 2, a(n) is an unused number that is as
 *          close as possible to a(n-1), shares a factor with a(n-1), and
 *          rad(a(n)*a(n-1)) has not previously occurred as the rad value of
 *          the product of adjacent terms, where rad(k) = A007947(k). If two
 *          such numbers exist then the smaller is...
 * https://oeis.org/A394421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 156, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1, a(2) = 2; for n > 2 the value a(n) is the unused integer
   that is as close as possible to a(n-1), shares a non‑trivial factor
   with a(n-1), and such that rad(a(n-1) * a(n)) has not occurred
   before as a rad value of a product of adjacent terms.  In case
   two candidates are equally close the smaller one is chosen. */
/* The function memoises the sequence so that calls with arbitrary order
   of indices are fast.  All arithmetic is performed in idx_t
   (uint64_t) and the result is cast to seq_t before being stored. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/* Radii ------------------------------------------------------------- */
/* ------------------------------------------------------------------ */

static idx_t rad_of(idx_t n) {
    idx_t rad = 1;
    if (n % 2ULL == 0ULL) {
        rad *= 2ULL;
        while (n % 2ULL == 0ULL) n /= 2ULL;
    }
    for (idx_t p = 3ULL; p * p <= n; p += 2ULL) {
        if (n % p == 0ULL) {
            rad *= p;
            while (n % p == 0ULL) n /= p;
        }
    }
    if (n > 1ULL) rad *= n;
    return rad;
}

static inline idx_t idx_gcd(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static inline idx_t rad_product_u64(idx_t x, idx_t y) {
    idx_t radx   = rad_of(x);
    idx_t rady   = rad_of(y);
    idx_t g      = idx_gcd(x, y);
    idx_t radg   = rad_of(g);
    unsigned __int128 prod = (unsigned __int128)radx * rady;
    prod /= radg;
    return (idx_t)prod;
}

/* ------------------------------------------------------------------ */
/* Rad list (rad(a(k)*a(k-1))) ------------------------------------- */
/* ------------------------------------------------------------------ */
static idx_t *rad_list = NULL;
static size_t rad_len  = 0;
static size_t rad_cap  = 0;

static void ensure_rad_capacity(void) {
    if (rad_len < rad_cap) return;
    size_t new_cap = rad_cap ? rad_cap << 1 : 64;
    rad_list = (idx_t *)realloc(rad_list, new_cap * sizeof(idx_t));
    rad_cap  = new_cap;
}

static bool rad_exists(idx_t r) {
    for (size_t i = 0; i < rad_len; ++i)
        if (rad_list[i] == r) return true;
    return false;
}

static void add_rad(idx_t r) {
    ensure_rad_capacity();
    rad_list[rad_len++] = r;
}

/* ------------------------------------------------------------------ */
/* Used numbers ----------------------------------------------------- */
/* ------------------------------------------------------------------ */
static unsigned char *used_flag = NULL;
static size_t used_cap          = 0;

static inline bool is_used(idx_t v) {
    return v < used_cap && used_flag[(size_t)v];
}

static inline void ensure_used_capacity(idx_t v) {
    if (v < used_cap) return;
    size_t new_cap = used_cap ? used_cap << 1 : 64;
    while (new_cap <= (size_t)v) new_cap <<= 1;
    unsigned char *new_arr = (unsigned char *)realloc(used_flag, new_cap);
    memset(new_arr + used_cap, 0, new_cap - used_cap);
    used_flag = new_arr;
    used_cap  = new_cap;
}

static inline void set_used(idx_t v) {
    ensure_used_capacity(v);
    used_flag[(size_t)v] = 1;
}

/* ------------------------------------------------------------------ */
/* Sequence cache --------------------------------------------------- */
/* ------------------------------------------------------------------ */
static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;

static void ensure_seq_capacity(size_t n) {
    if (n <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 64;
    while (new_cap < n) new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

/* ------------------------------------------------------------------ */
/* Compute the next term (for n >= 3) -------------------------------- */
/* ------------------------------------------------------------------ */
static seq_t compute_next(idx_t prev) {
    /* Search distance by distance */
    for (idx_t d = 1; /* no bound; will terminate */ ; ++d) {
        /* Lower candidate */
        if (prev > d) {
            idx_t cand = prev - d;
            if (!is_used(cand) && idx_gcd(prev, cand) != 1) {
                idx_t rad_val = rad_product_u64(prev, cand);
                if (!rad_exists(rad_val)) return (seq_t)cand;
            }
        }
        /* Upper candidate */
        idx_t cand = prev + d;
        if (!is_used(cand) && idx_gcd(prev, cand) != 1) {
            idx_t rad_val = rad_product_u64(prev, cand);
            if (!rad_exists(rad_val)) return (seq_t)cand;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Main function required by the framework --------------------------- */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* undefined for n = 0 */

    /* Initialise only once */
    if (seq_len == 0) {
        ensure_seq_capacity(2);
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)2;
        seq_len = 2;
        set_used(1);
        set_used(2);
        add_rad(rad_product_u64(1, 2));
    }

    /* Compute until we reach desired index */
    while (seq_len < (size_t)n) {
        seq_t prev_val    = seq_cache[seq_len - 1];
        idx_t  prev       = (idx_t)prev_val;
        seq_t cand_val    = compute_next(prev);
        idx_t cand        = (idx_t)cand_val;

        ensure_seq_capacity(seq_len + 1);
        seq_cache[seq_len++] = cand_val;
        set_used(cand);

        idx_t rad_val = rad_product_u64(prev, cand);
        add_rad(rad_val);
    }

    return seq_cache[(size_t)n - 1];
}
