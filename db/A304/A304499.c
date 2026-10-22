/*
 * A304499  Solution (c(n)) of the system of complementary equations defined
 *          in Comments.
 * https://oeis.org/A304499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, b(0)=2, c(0)=4; for n≥1, a(n)=least unused positive
 *            integer, b(n)=next least unused, c(n)=2*a(n)+b(n). This function
 *            returns c(n)
 * Range:     u8 n <= 35, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint8_t *used_bits = NULL;
static size_t used_bits_cap_bits = 0;
static size_t used_next = 1;

/* Ensure bitset can cover bit 'idx'. */
static void ensure_used_capacity(size_t idx)
{
    if (idx >= used_bits_cap_bits) {
        size_t new_bits  = idx + 64;                      /* grow by 64 bits */
        size_t new_bytes = (new_bits + 7) >> 3;          /* bytes needed */
        size_t old_bytes = used_bits_cap_bits >> 3;
        used_bits = (uint8_t *)realloc(used_bits, new_bytes);
        memset(used_bits + old_bytes, 0, new_bytes - old_bytes);
        used_bits_cap_bits = new_bits;
    }
}

/* Query bit 'idx'. */
static inline int used_get(size_t idx)
{
    if (idx >= used_bits_cap_bits) return 0;
    return (used_bits[idx >> 3] >> (idx & 7)) & 1;
}

/* Set bit 'idx'. */
static inline void used_set(size_t idx)
{
    ensure_used_capacity(idx);
    used_bits[idx >> 3] |= (uint8_t)(1u << (idx & 7));
}

static seq_t *c_vals = NULL;
static size_t c_len = 0;
static size_t c_cap = 0;

/* Ensure array can hold index 'idx'. */
static void ensure_c_capacity(size_t idx)
{
    if (idx + 1 > c_cap) {
        size_t new_cap = (idx + 1) * 2;
        c_vals = (seq_t *)realloc(c_vals, new_cap * sizeof(seq_t));
        c_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    size_t target = (size_t)n;
    if (target < c_len) return c_vals[target];

    for (size_t i = c_len; i <= target; ++i) {
        /* find a(i) */
        for (; used_get(used_next); ++used_next);
        seq_t A = (seq_t)used_next;
        used_set(used_next++);
        /* find b(i) */
        for (; used_get(used_next); ++used_next);
        seq_t B = (seq_t)used_next;
        used_set(used_next++);
        /* compute c(i) */
        seq_t C = (seq_t)2 * A + B;
        used_set((size_t)C);
        /* store */
        ensure_c_capacity(i);
        c_vals[i] = C;
    }

    c_len = target + 1;
    return c_vals[target];
}
