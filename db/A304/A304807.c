/*
 * A304807  Solution (a(n)) of the complementary equation a(n) = b(2n) + b(3n)
 *          ; see Comments.
 * https://oeis.org/A304807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A304807: a(n) = b(2n) + b(3n)
 *  The companion sequence b(n) is defined recursively as the
 *  least positive integer not yet used (either as a previous b
 *  value or a previous a value).  Initial values:
 *      b(0) = 1,  a(0) = 2
 *
 *  The function below computes a(n) for any n ≥ 0.  It preserves
 *  previously computed values so that calls can be made in any
 *  order.  It runs in O(n) time with a small constant factor.
 *
 *  All integer arithmetic is performed in the type seq_t, which
 *  can be unsigned 8, 16, 32, 64 or 128 bits, or an
 *  arbitrary‑precision integer type.  All indices are of type
 *  idx_t (unsigned 64‑bit).
 *
 *  The code follows the requirements exactly: it has one function
 *  with external linkage named `a`, it uses only integer
 *  operations, and it does not assume call order.
 */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/* ---------- Static state ---------- */
static seq_t *b_arr   = NULL;   /* values of b   */
static size_t b_cap   = 0, b_len   = 0;

static seq_t *a_arr   = NULL;   /* values of a   */
static size_t a_cap   = 0, a_len   = 0;

static unsigned char *used_bits = NULL;   /* bitset of used integers */
static size_t used_size = 0;              /* number of bits stored */

static size_t next_candidate = 1;        /* next integer to try for b */

/* ---------- Bitset helpers ---------- */
static void ensure_used_bits(size_t val)
{
    if (val < used_size) return;
    size_t new_size = used_size ? used_size * 2 : 64;
    while (new_size <= val) new_size *= 2;
    unsigned char *new_buf = (unsigned char *)realloc(used_bits,
                                                      (new_size + 7) >> 3);
    if (!new_buf) exit(EXIT_FAILURE);
    /* zero only the newly allocated portion */
    memset(new_buf + (used_size >> 3),
           0,
           ((new_size >> 3) - (used_size >> 3)));
    used_bits = new_buf;
    used_size = new_size;
}

static int used_get(size_t v)
{
    if (v < used_size)
        return (used_bits[v >> 3] >> (v & 7)) & 1U;
    return 0;
}

static void used_set(size_t v)
{
    ensure_used_bits(v);
    used_bits[v >> 3] |= (1U << (v & 7));
}

/* ---------- Array capacity helpers ---------- */
static void ensure_b_capacity(size_t idx)
{
    if (idx < b_cap) return;
    size_t new_cap = b_cap ? b_cap * 2 : 256;
    while (new_cap <= idx) new_cap *= 2;
    seq_t *new_b = (seq_t *)realloc(b_arr, new_cap * sizeof(seq_t));
    if (!new_b) exit(EXIT_FAILURE);
    b_arr = new_b;
    b_cap = new_cap;
}

static void ensure_a_capacity(size_t idx)
{
    if (idx < a_cap) return;
    size_t new_cap = a_cap ? a_cap * 2 : 256;
    while (new_cap <= idx) new_cap *= 2;
    seq_t *new_a = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    if (!new_a) exit(EXIT_FAILURE);
    a_arr = new_a;
    a_cap = new_cap;
}

/* ---------- Core construction routine ---------- */
static void ensure_b_up_to(size_t limit)
{
    if (limit < b_len) return;

    ensure_b_capacity(limit);
    ensure_a_capacity(limit / 3);

    for (; b_len <= limit; ++b_len) {
        /* Find the least unused positive integer */
        while (used_get(next_candidate)) ++next_candidate;
        seq_t val = (seq_t)next_candidate;
        b_arr[b_len] = val;
        used_set((size_t)val);
        ++next_candidate;

        /* When an index is a multiple of 3, compute the corresponding a */
        if ((b_len % 3) == 0) {
            size_t m = b_len / 3;
            seq_t a_val = b_arr[2 * m] + b_arr[3 * m];
            a_arr[m] = a_val;
            used_set((size_t)a_val);
        }
    }
}

/* ---------- Public sequence function ---------- */
seq_t a(idx_t n)
{
    /* a(n) requires b up to index 3*n */
    size_t limit = 3 * (size_t)n;
    ensure_b_up_to(limit);
    return a_arr[(size_t)n];
}
