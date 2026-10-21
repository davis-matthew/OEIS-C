/*
 * A299970  Lexicographic first sequence of nonnegative integers such that
 *          a(n) + a(n+1) has a digit 0, and no term occurs twice.
 * https://oeis.org/A299970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(0)=0. For n>0, a(n) is the smallest non‑negative integer not used
   before that makes a(n‑1)+a(n) contain the decimal digit 0. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* seq_t and idx_t are supplied by the build system. */
typedef idx_t idx_t;
typedef seq_t seq_t;

/* ---------- Storage -------------------------------------------------- */
static seq_t *seq      = NULL;
static size_t seq_len  = 0;   /* logical length of seq[0..seq_len-1]       */
static size_t seq_cap  = 0;   /* physical capacity of seq array            */

static uint8_t *used    = NULL;   /* bitmap of values already used            */
static size_t   used_cap = 0;      /* capacity of used array                   */

/* ---------- Helpers ------------------------------------------------- */
static void ensure_seq_capacity_to(size_t idx)
{
    if (seq_cap > idx) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap <= idx) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

static void ensure_used_capacity_to(size_t idx)
{
    if (used_cap > idx) return;
    size_t new_cap = used_cap ? used_cap * 2 : 16;
    while (new_cap <= idx) new_cap <<= 1;
    used = (uint8_t *)realloc(used, new_cap * sizeof(uint8_t));
    for (size_t i = used_cap; i < new_cap; ++i)
        used[i] = 0;
    used_cap = new_cap;
}

/* True if decimal representation of v contains the digit 0. */
static bool has_zero_digit(seq_t v)
{
    if (v == 0) return true;
    while (v != 0) {
        if ((v % 10) == 0) return true;
        v /= 10;
    }
    return false;
}

/* Generate the sequence up to index n (inclusive). */
static void generate_to(idx_t n)
{
    /* Ensure storage for results. */
    ensure_seq_capacity_to(n);

    for (idx_t i = seq_len; i <= n; ++i) {
        if (i == 0) {
            seq[0] = 0;
            ensure_used_capacity_to(0);
            used[0] = 1;
        } else {
            seq_t prev = seq[i - 1];
            size_t cand = 0;  /* start searching from 0 */
            for (;;) {
                if (cand >= used_cap) ensure_used_capacity_to(cand);
                if (used[cand]) { ++cand; continue; }
                seq_t sum = prev + (seq_t)cand;
                if (has_zero_digit(sum)) break;
                ++cand;
            }
            used[cand] = 1;
            seq[i] = (seq_t)cand;
        }
    }
    seq_len = (size_t)n + 1;
}

/* ---------- Public API -------------------------------------------- */
seq_t a(idx_t n)
{
    if ((size_t)n >= seq_len) generate_to(n);
    return seq[(size_t)n];
}
