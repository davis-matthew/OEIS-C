/*
 * A357504  Numbers that are the sum of two distinct triangular numbers.
 * https://oeis.org/A357504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = numbers that can be written as the sum of two distinct
 *            triangular numbers
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- internal state ---------- */
static idx_t flag_size = 0;            /* size of the triangular flag array */
static uint8_t *tri_flag = NULL;       /* tri_flag[t] == 1 iff t is triangular */

static seq_t *triang = NULL;           /* list of triangular numbers (ascending) */
static size_t triang_len = 0;
static size_t triang_cap = 0;
static idx_t tri_k = 0;                /* next k (starting at 0) to generate */

static seq_t *cache = NULL;            /* cached sequence terms */
static size_t cache_cap = 0;
static size_t cache_len = 0;

static idx_t max_checked = 0;          /* largest m examined so far */

/* ---------- helpers for dynamic arrays ---------- */

static void ensure_flag_size(idx_t needed)
{
    if (needed <= flag_size) return;
    size_t new_size = (size_t)needed;
    uint8_t *new_flag = (uint8_t *)realloc(tri_flag, new_size);
    if (!new_flag) exit(EXIT_FAILURE);
    for (size_t i = flag_size; i < new_size; ++i) new_flag[i] = 0;
    tri_flag = new_flag;
    flag_size = new_size;
}

static void ensure_triang_capacity(size_t needed)
{
    if (needed <= triang_cap) return;
    size_t new_cap = triang_cap ? 2 * triang_cap : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(triang, new_cap * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);
    triang = new_arr;
    triang_cap = new_cap;
}

static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? 2 * cache_cap : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);
    cache = new_arr;
    cache_cap = new_cap;
}

/* ---------- precompute triangular numbers up to max_m ---------- */
static void precompute_triangulars(idx_t max_m)
{
    while (true) {
        /* T_k = k*(k+1)/2 */
        seq_t t = (seq_t)tri_k * ((seq_t)(tri_k + 1)) / 2;
        if (t > (seq_t)max_m) break;          /* stop once we exceed max_m */
        idx_t ti = (idx_t)t;
        ensure_flag_size(ti + 1);
        tri_flag[ti] = 1;
        ensure_triang_capacity(triang_len + 1);
        triang[triang_len++] = t;
        ++tri_k;
    }
}

/* ---------- test if m can be expressed as sum of two distinct triangular numbers ---------- */
static bool is_sum_of_two_distinct_triangular(idx_t m)
{
    precompute_triangulars(m);           /* ensure T_k <= m are known */
    for (size_t i = 0; i < triang_len; ++i) {
        idx_t ti = (idx_t)triang[i];
        if (ti > m) break;               /* triangular numbers are sorted */
        seq_t rem = (seq_t)m - triang[i];
        idx_t remi = (idx_t)rem;
        if (remi == ti) continue;        /* distinctness required */
        if (remi < flag_size && tri_flag[remi]) return true;
    }
    return false;
}

/* ---------- generate terms up to target index (1‑based) ---------- */
static void generate_up_to(idx_t target)
{
    while (cache_len < target) {
        ++max_checked;
        if (is_sum_of_two_distinct_triangular(max_checked)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)max_checked;
        }
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety; offset is 1 */
    generate_up_to(n);
    return cache[n - 1];
}
