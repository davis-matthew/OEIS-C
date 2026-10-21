/*
 * A242704  Lexicographically earliest sequence of distinct positive integers
 *          such that for any n>0 the number of ones in the binary
 *          representation of a(n) divides a(n+1).
 * https://oeis.org/A242704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 167, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest sequence of distinct positive integers
   such that for every n>0 the number of ones in the binary
   representation of a(n) divides a(n+1).
   Recurrence: a(1)=1; for n>=1,
   a(n+1) = the smallest unused positive integer that is a multiple
   of popcount(a(n)). */

/* The implementation uses dynamic arrays for the sequence itself,
   a used‑flag array, and a table of next candidates for each weight.
   All indices are handled with unsigned 64‑bit types as required. */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- Popcount for seq_t ---------- */
static size_t popcount_seq(seq_t x)
{
    size_t c = 0;
    while (x) {
        c += (size_t)(x & (seq_t)1);
        x >>= 1;
    }
    return c;
}

/* ---------- Dynamic storage for the sequence ---------- */
static seq_t  *seq      = NULL;   /* generated terms */
static size_t seq_len   = 0;      /* number of terms stored */
static size_t seq_cap   = 0;      /* capacity of seq array */

/* ---------- Array marking used integers ---------- */
static uint8_t *used    = NULL;
static size_t  used_cap = 0;

/* ---------- Next unused multiple for each weight ---------- */
static seq_t *next_mult = NULL;
static size_t next_cap  = 0;

/* ---------- Resize helpers ---------- */
static void resize_seq(size_t min_needed)
{
    if (min_needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < min_needed) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

static void resize_used(size_t min_needed)
{
    if (min_needed <= used_cap) return;
    size_t new_cap = used_cap ? used_cap << 1 : 16;
    while (new_cap < min_needed) new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(used, new_cap);
    for (size_t i = used_cap; i < new_cap; ++i) tmp[i] = 0;
    used = tmp;
    used_cap = new_cap;
}

static void ensure_next_capacity(void)
{
    if (next_cap != 0) return;
    next_cap = 256;                           /* initial capacity  */
    next_mult = (seq_t *)malloc(next_cap * sizeof(seq_t));
    for (size_t i = 0; i < next_cap; ++i) next_mult[i] = (seq_t)i;
}

/* ---------- Get smallest unused multiple of weight w ---------- */
static seq_t get_next_for_weight(size_t w)
{
    ensure_next_capacity();
    if (w >= next_cap) {
        size_t old_cap = next_cap;
        size_t new_cap = next_cap ? next_cap << 1 : 256;
        while (new_cap <= w) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(next_mult, new_cap * sizeof(seq_t));
        for (size_t i = old_cap; i < new_cap; ++i) tmp[i] = (seq_t)i;
        next_mult = tmp;
        next_cap  = new_cap;
    }

    seq_t cand = next_mult[w];
    if (cand == 0) cand = (seq_t)w;           /* safety for uninitialized weight */

    while (1) {
        size_t idx = (size_t)cand;            /* used array index */
        resize_used(idx + 1);
        if (!used[idx]) {
            used[idx] = 1;
            next_mult[w] = cand + (seq_t)w;
            return cand;
        }
        cand += (seq_t)w;
    }
}

/* ---------- Generate the sequence up to index n ---------- */
static void build_upto(idx_t n)
{
    if (seq_len >= (size_t)n) return;

    /* Initialise if first call */
    if (seq_len == 0) {
        resize_seq(1);
        seq[0] = (seq_t)1;
        seq_len = 1;
        resize_used(2);             /* need index 1 */
        used[1] = 1;
    }

    while (seq_len < (size_t)n) {
        seq_t last = seq[seq_len - 1];
        size_t w = popcount_seq(last);
        seq_t next_val = get_next_for_weight(w);
        resize_seq(seq_len + 1);
        seq[seq_len++] = next_val;
    }
}

/* ---------- Public interface required by the judge ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined below offset */
    build_upto(n);
    return seq[(size_t)n - 1];
}
