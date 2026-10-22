/*
 * A344538  Lexicographically earliest sequence such that |a(n+1)-a(n)| is a
 *          cube > 1 and no number occurs twice; a(0) = 0.
 * https://oeis.org/A344538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0. For n>0: a(n)=a(n‑1)−27 if that value is ≥0 and unused,
 *            * otherwise a(n)=a(n‑1)+8. This greedy rule produces the
 *            lexicographically * earliest permutation of the non‑negative
 *            integers whose successive * differences are +8 or –27. The
 *            sequence is periodic with period 35 * (a(n+35)=a(n)+35)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Static storage for the sequence and the used‑number bitmap. */
static seq_t  *seq_buf   = NULL;
static size_t seq_cap   = 0;   /* allocated capacity of seq_buf   */
static size_t cache_len = 0;   /* number of terms already computed */

static uint8_t *used_buf = NULL;
static size_t   used_cap = 0;  /* allocated capacity of used_buf  */

/* Ensure seq_buf has capacity at least n+1. */
static void ensure_seq_capacity(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (seq_cap >= need) return;

    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_buf = (seq_t *)realloc(seq_buf, new_cap * sizeof(seq_t));
    if (!seq_buf) exit(1);     /* allocation failed */
    seq_cap = new_cap;
}

/* Ensure used_buf can index up to value v (inclusive). */
static void ensure_used_capacity(seq_t v)
{
    size_t vv = (size_t)v;
    if (used_cap > vv) return;
    size_t new_cap = used_cap ? used_cap * 2 : 1024;
    if (new_cap <= vv) new_cap = vv + 1;
    uint8_t *new_buf = (uint8_t *)realloc(used_buf, new_cap);
    if (!new_buf) exit(1);
    /* zero the newly allocated region */
    if (new_cap > used_cap)
        memset(new_buf + used_cap, 0, new_cap - used_cap);
    used_buf = new_buf;
    used_cap = new_cap;
}

/* Lazily initialise the buffers on the first call. */
static void init_buffers(void)
{
    if (!seq_buf) {
        seq_cap = 16;
        seq_buf = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq_buf) exit(1);
        seq_buf[0] = 0;
        cache_len = 1;
    }
    if (!used_buf) {
        used_cap = 1024;
        used_buf = (uint8_t *)calloc(used_cap, 1);
        if (!used_buf) exit(1);
    }
    /* mark the first term as used */
    ensure_used_capacity(0);
    used_buf[0] = 1;
}

/* Public function required by the challenge */
seq_t a(idx_t n)
{
    if (!seq_buf) init_buffers();

    /* If the requested index is already known, return it. */
    if ((idx_t)cache_len > n)
        return seq_buf[n];

    /* Expand seq_buf to hold up to n (inclusive). */
    ensure_seq_capacity(n);

    /* Generate missing terms one by one. */
    for (idx_t i = cache_len; i <= (idx_t)n; ++i) {
        seq_t prev     = seq_buf[i - 1];
        seq_t candidate = prev;      /* placeholder */
        int   use_minus = 0;

        /* Try the –27 option first if it is valid. */
        if (prev >= (seq_t)27) {
            candidate = prev - (seq_t)27;
            ensure_used_capacity(candidate);
            if (!used_buf[(size_t)candidate])
                use_minus = 1;
        }

        if (use_minus) {
            seq_buf[i] = candidate;
        } else {
            seq_buf[i] = prev + (seq_t)8;
        }

        /* Mark the new value as used. */
        ensure_used_capacity(seq_buf[i]);
        used_buf[(size_t)seq_buf[i]] = 1;
    }

    cache_len = (size_t)n + 1;
    return seq_buf[n];
}
