/*
 * A329390  Solution sequence (a(n)) of the complementary equation a(n) = 6
 *          b(n+1) - 2 b(n), with b(0) = 1.
 * https://oeis.org/A329390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A329390 –   a(n) = 6 b(n+1) – 2 b(n)
 *  where b(0) = 1 and the pairs (a(n)) and (b(n)) form a complementary
 *  partition of the positive integers.  The function builds the terms
 *  lazily and caches them; any order of calls is supported.
 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ---------- dynamic array helpers ---------- */
static void grow_seq(seq_t **arr, size_t *len, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = *cap ? *cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    *arr = tmp;
    *cap = new_cap;
}

/* ---------- used‑tracker (bitmap) ---------- */
static unsigned char *used_bits   = NULL;   /* bitmap of used numbers */
static size_t        used_bytes  = 0;      /* bytes allocated for bitmap */

static void ensure_used(idx_t need)
{
    if (need < used_bytes * 8) return;
    size_t new_bytes = used_bytes ? used_bytes * 2 : 8;
    while (new_bytes * 8 <= need) new_bytes <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(used_bits, new_bytes);
    if (!tmp) exit(1);
    memset(tmp + used_bytes, 0, new_bytes - used_bytes);
    used_bits = tmp;
    used_bytes = new_bytes;
}

static void used_set(idx_t v)
{
    ensure_used(v);
    used_bits[v >> 3] |= (unsigned char)(1 << (v & 7));
}

static bool used_get(idx_t v)
{
    if (v >= used_bytes * 8) return false;
    return (used_bits[v >> 3] & (unsigned char)(1 << (v & 7))) != 0;
}

/* ---------- global caches ---------- */
static seq_t  *a_arr   = NULL;
static seq_t  *b_arr   = NULL;
static size_t a_len  = 0, a_cap  = 0;
static size_t b_len  = 0, b_cap  = 0;

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    /* Lazy initialisation */
    if (!a_arr) {
        a_cap = 16;  a_arr = (seq_t *)malloc(a_cap * sizeof(seq_t));
        b_cap = 16;  b_arr = (seq_t *)malloc(b_cap * sizeof(seq_t));
        if (!a_arr || !b_arr) exit(1);

        /* initialise b(0) = 1 */
        seq_t b0 = (seq_t)1;
        b_arr[b_len++] = b0;
        used_set((idx_t)b0);
    }

    /* Generate terms up to a(n) */
    while (a_len <= n) {
        /* current last b used to generate the next pair */
        seq_t last_b = b_arr[b_len - 1];

        /* find the smallest integer > last_b that is unused */
        idx_t cand = (idx_t)last_b + 1;
        while (used_get(cand)) ++cand;
        seq_t new_b = (seq_t)cand;

        /* store b(k+1) */
        grow_seq(&b_arr, &b_len, &b_cap, b_len + 1);
        b_arr[b_len++] = new_b;
        used_set(cand);

        /* compute a(k) = 6*b(k+1) – 2*b(k) */
        seq_t prev_b = b_arr[b_len - 2];
        seq_t new_a = (seq_t)6 * new_b - (seq_t)2 * prev_b;

        /* store a(k) */
        grow_seq(&a_arr, &a_len, &a_cap, a_len + 1);
        a_arr[a_len++] = new_a;
        used_set((idx_t)new_a);
    }

    return a_arr[n];
}
