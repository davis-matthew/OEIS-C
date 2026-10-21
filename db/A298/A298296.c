/*
 * A298296  Solution b( ) of the complementary equation a(n) = a(0)*b(n) +
 *          a(1)*b(n-1), where a(0) = 1, a(1) = 2, b(0) = 3, b(1) = 4, b(2) =
 *          5, and (b(n)) is the increasing sequence of positive integers not
 *          in (a(n)). See Comments.
 * https://oeis.org/A298296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   b(n) is the increasing sequence of positive integers not in
 *            a(n), where a(0) = 1, a(1) = 2 and a(n) = a(0)*b(n) +
 *            a(1)*b(n-1) = b(n) + 2*b(n-1)
 * Range:     u8 n <= 120, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a(idx_t n) returns the nth term of that complementary sequence. */

#include <stdlib.h>
#include <stdint.h>

/* ----- Bitset of used numbers (a and b) ----- */
static uint8_t *used_bits = NULL;   /* bitmap of used values   */
static size_t used_bytes = 0;       /* allocated bitmap size (bytes) */

static void grow_used_if_needed(uint64_t v)
{
    size_t required_bytes = (v >> 3) + 1;
    if (required_bytes <= used_bytes)
        return;

    size_t new_bytes = used_bytes > 0 ? used_bytes : 8;
    while (new_bytes < required_bytes)
        new_bytes <<= 1;

    uint8_t *tmp = (uint8_t *)realloc(used_bits, new_bytes);
    if (!tmp)
        abort();
    /* clear the newly allocated part */
    if (new_bytes > used_bytes)
        for (size_t i = used_bytes; i < new_bytes; ++i)
            tmp[i] = 0;

    used_bits = tmp;
    used_bytes = new_bytes;
}

static void mark_used(uint64_t v)
{
    grow_used_if_needed(v);
    used_bits[v >> 3] |= (uint8_t)(1U << (v & 7));
}

static int is_used(uint64_t v)
{
    if ((v >> 3) >= used_bytes)
        return 0;
    return (used_bits[v >> 3] >> (v & 7)) & 1U;
}

/* ----- Sequence caches ----- */
static seq_t *a_cache = NULL;      /* a(·) values   */
static seq_t *b_cache = NULL;      /* b(·) values   */
static size_t a_capacity = 0;
static size_t b_capacity = 0;
static idx_t a_max = 0;            /* largest computed a index */
static idx_t b_max = 0;            /* largest computed b index */
static uint64_t next_b_candidate = 6;  /* next integer to try for b */

/* Ensure the a cache can hold at least n+1 elements. */
static void ensure_a_capacity(idx_t n)
{
    if (n < a_capacity)
        return;
    size_t new_cap = a_capacity > 0 ? a_capacity : 4;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    a_cache = tmp;
    a_capacity = new_cap;
}

/* Ensure the b cache can hold at least n+1 elements. */
static void ensure_b_capacity(idx_t n)
{
    if (n < b_capacity)
        return;
    size_t new_cap = b_capacity > 0 ? b_capacity : 4;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    b_cache = tmp;
    b_capacity = new_cap;
}

/* ----- Initialization ----- */
static int initialized = 0;

static void init_sequence(void)
{
    /* Allocate space for the initial terms */
    a_capacity = 4;   /* indices 0..3 will fit */
    b_capacity = 4;   /* indices 0..3 will fit */
    a_cache = (seq_t *)malloc(a_capacity * sizeof(seq_t));
    b_cache = (seq_t *)malloc(b_capacity * sizeof(seq_t));
    if (!a_cache || !b_cache)
        abort();

    /* Base values for a */
    a_cache[0] = (seq_t)1;
    a_cache[1] = (seq_t)2;
    a_max = 1;
    mark_used((uint64_t)1);
    mark_used((uint64_t)2);

    /* First three terms of b */
    b_cache[0] = (seq_t)3;
    b_cache[1] = (seq_t)4;
    b_cache[2] = (seq_t)5;
    b_max = 2;
    mark_used((uint64_t)3);
    mark_used((uint64_t)4);
    mark_used((uint64_t)5);

    /* Compute a(2) from the recurrence */
    seq_t a2 = b_cache[2] + (seq_t)(2) * b_cache[1];  /* 1*5 + 2*4 = 13 */
    a_cache[2] = a2;
    a_max = 2;
    mark_used((uint64_t)a2);

    /* Set up next candidate for b (start from 6) */
    next_b_candidate = 6;

    initialized = 1;
}

/* ----- Compute terms up to n ----- */
static void compute_up_to(idx_t n)
{
    if (!initialized)
        init_sequence();
    if (n <= a_max)
        return;

    /* We will generate b(i) and a(i) for i = b_max+1 .. n */
    for (idx_t i = b_max + 1; i <= n; ++i)
    {
        /* Ensure capacity */
        ensure_b_capacity(i);
        ensure_a_capacity(i);

        /* Find smallest unused integer for b(i) */
        while (is_used(next_b_candidate))
            ++next_b_candidate;
        seq_t b_val = (seq_t)next_b_candidate;
        b_cache[i] = b_val;
        mark_used((uint64_t)b_val);
        ++next_b_candidate;  /* next trial */

        /* Compute a(i) = b(i) + 2*b(i-1) (for i >= 2) */
        seq_t a_val;
        if (i == 0)
            a_val = a_cache[0];
        else if (i == 1)
            a_val = a_cache[1];
        else
        {
            a_val = b_cache[i] + (seq_t)(2) * b_cache[i - 1];
            mark_used((uint64_t)a_val);
        }
        a_cache[i] = a_val;

        /* Update maxima */
        a_max = i;
        b_max = i;
    }
}

/* ----- Public function ----- */
seq_t a(idx_t n)
{
    /* Return the nth term of the complementary sequence b(n). */
    compute_up_to(n);
    return b_cache[n];
}
