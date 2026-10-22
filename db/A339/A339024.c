/*
 * A339024  a(1) = 1, a(n) is the least m not already in the sequence whose
 *          binary expansion begins with the binary expansion of the binary
 *          weight of a(n-1).
 * https://oeis.org/A339024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 1; for n > 1, let w = wt(a(n-1)) = number of 1s in
 *            binary(a(n-1)). a(n) is the least integer m not already
 *            appearing in the sequence whose binary representation starts
 *            with the binary representation of w. This code does not use a
 *            closed formula but generates the sequence on demand and memo...
 * Range:     u8 n <= 165, u16 n <= 9952, u32 n <= 9952, u64 n <= 9952,
 *            u128 n <= 9952, big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* utilities that are provided by the build system */
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free_array.h"

/* helper: number of set bits in a 64‑bit integer */
static inline unsigned popcount64(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    /* Fallback to simple loop if compiler does not provide builtin */
    unsigned count = 0;
    while (x) { x &= x - 1; ++count; }
    return count;
#endif
}

/* helper: bit length of a 64‑bit integer (1 for x==0) */
static inline unsigned bitlen64(uint64_t x)
{
    if (x == 0) return 1;
    return 64 - __builtin_clzll(x);
}

/* helper: does c's binary representation begin with the binary of w? */
static bool starts_with_prefix(uint64_t c, uint64_t w)
{
    unsigned len_w = bitlen64(w);
    unsigned len_c = bitlen64(c);
    if (len_c < len_w) return false;
    unsigned shift = len_c - len_w;
    return (c >> shift) == w;
}

/* ------------------------------------------------------------------ */
/* Simple hash table for used numbers -------------------------------- */

#define EMPTY_VALUE 0ULL   /* 0 is never used as a sequence term */

static uint64_t *used_table = NULL;
static size_t used_cap = 0;     /* power of two */
static size_t used_cnt = 0;

/* insert value into hash table (value is guaranteed not to be present) */
static void used_insert(uint64_t val)
{
    if (used_cap == 0) {
        used_cap = 16;
        used_table = (uint64_t*)calloc(used_cap, sizeof(uint64_t));
    }
    /* grow if load factor would exceed 0.5 */
    if ((used_cnt + 1) > used_cap / 2) {
        size_t newcap = used_cap * 2;
        uint64_t *newtab = (uint64_t*)calloc(newcap, sizeof(uint64_t));
        /* rehash existing elements */
        for (size_t i = 0; i < used_cap; ++i) {
            uint64_t old = used_table[i];
            if (old != EMPTY_VALUE) {
                size_t idx = old & (newcap - 1);
                while (newtab[idx] != EMPTY_VALUE) idx = (idx + 1) & (newcap - 1);
                newtab[idx] = old;
            }
        }
        free(used_table);
        used_table = newtab;
        used_cap = newcap;
    }
    /* insert new value */
    size_t idx = val & (used_cap - 1);
    while (used_table[idx] != EMPTY_VALUE) idx = (idx + 1) & (used_cap - 1);
    used_table[idx] = val;
    ++used_cnt;
}

/* check if value is present in hash table */
static bool used_contains(uint64_t val)
{
    if (used_cap == 0) return false;
    size_t idx = val & (used_cap - 1);
    while (used_table[idx] != EMPTY_VALUE) {
        if (used_table[idx] == val) return true;
        idx = (idx + 1) & (used_cap - 1);
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Sequence storage --------------------------------------------------- */

static seq_t *seq_buf = NULL;
static size_t seq_len = 0;   /* number of terms stored */
static size_t seq_cap = 0;

/* grow sequence buffer if needed (simple reallocation using provided util) */
static void seq_grow_if_needed(size_t need)
{
    if (need <= seq_cap) return;
    size_t newcap = seq_cap ? seq_cap * 2 : 16;
    while (newcap < need) newcap *= 2;
    seq_t *newbuf = alloc_seq_array(newcap);
    for (size_t i = 0; i < seq_len; ++i) newbuf[i] = seq_buf[i];
    seq_free_array(seq_buf);
    seq_buf = newbuf;
    seq_cap = newcap;
}

/* ------------------------------------------------------------------ */
/* Main function ------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for 0, but return 0 */

    /* initialize on first call */
    if (seq_len == 0) {
        seq_grow_if_needed(1);
        seq_buf[0] = (seq_t)1;
        seq_len = 1;
        used_insert(1);            /* mark 1 as used */
    }

    /* extend sequence up to required index */
    while (seq_len < (size_t)n) {
        uint64_t prev = (uint64_t)seq_buf[seq_len - 1];
        uint64_t w    = (uint64_t)popcount64(prev);

        uint64_t cand = 1;
        while (true) {
            if (!used_contains(cand) && starts_with_prefix(cand, w)) break;
            ++cand;
        }

        used_insert(cand);
        seq_grow_if_needed(seq_len + 1);
        seq_buf[seq_len] = (seq_t)cand;
        ++seq_len;
    }

    return seq_buf[(size_t)n - 1];
}
