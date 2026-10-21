/*
 * A294860  Solution of the equation a(n) = a(n-2) + b(n-2), where a( ) and b(
 *          ) are increasing sequences of positive integers such that every
 *          positive integer is in one of them and only one term is in both.
 * https://oeis.org/A294860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + b(n-2), with a(0)=1, a(1)=2, b(0)=3. The
 *            sequences a() and b() are increasing, complementary (every
 *            positive integer occurs in exactly one, except 4 occurs in
 *            both). This implementation builds the terms on demand and
 *            memoises all previously computed values
 * Range:     u8 n <= 26, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* storage for the a‑sequence */
static seq_t *a_seq = NULL;
static size_t  a_len = 0;   /* current number of terms stored */
static size_t  a_cap = 0;   /* allocated capacity */

/* storage for the b‑sequence */
static seq_t *b_seq = NULL;
static size_t  b_len = 0;
static size_t  b_cap = 0;

/* bitset that marks numbers that are already in a() */
static uint8_t *a_bits = NULL;
static size_t   bit_bytes = 0;   /* capacity in bytes */

/* ------------------------------------------------------------------ */
/*  Helper to ensure capacity for a_seq                            */
static void ensure_a_cap(size_t needed)
{
    if (needed <= a_cap) return;
    size_t new_cap = (a_cap > 0) ? a_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(a_seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    a_seq = tmp;
    a_cap = new_cap;
}

/* Helper to ensure capacity for b_seq                            */
static void ensure_b_cap(size_t needed)
{
    if (needed <= b_cap) return;
    size_t new_cap = (b_cap > 0) ? b_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(b_seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    b_seq = tmp;
    b_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Functions to handle the bitset that records numbers in a()        */
static void ensure_bitset_cap(seq_t v)
{
    /* we need bits 0 .. v inclusive */
    idx_t bits_needed = (idx_t)v + 1;
    size_t bytes_needed = (size_t)((bits_needed + 7) >> 3);
    if (bytes_needed <= bit_bytes) return;
    uint8_t *tmp = (uint8_t *)realloc(a_bits, bytes_needed);
    if (!tmp) abort();
    /* zero newly allocated part */
    if (bytes_needed > bit_bytes)
        memset(tmp + bit_bytes, 0, bytes_needed - bit_bytes);
    a_bits = tmp;
    bit_bytes = bytes_needed;
}

static void set_bit(seq_t v)
{
    /* mark number v as present in a() */
    idx_t idx = (idx_t)v;
    ensure_bitset_cap(v);
    a_bits[idx >> 3] |= (uint8_t)(1U << (idx & 7));
}

static int test_bit(seq_t v)
{
    /* test whether v is present in a() */
    idx_t idx = (idx_t)v;
    if (((size_t)(idx >> 3)) >= bit_bytes) return 0;
    return (a_bits[idx >> 3] >> (idx & 7)) & 1U;
}

/* ------------------------------------------------------------------ */
/*  One‑time initialisation of the data structures                    */
static void init_data(void)
{
    if (a_seq != NULL) return;  /* already initialised */

    /* a sequence – start with 1 and 2 */
    a_cap = 8;
    a_seq = (seq_t *)calloc(a_cap, sizeof(seq_t));
    if (!a_seq) abort();
    a_seq[0] = (seq_t)1;
    a_seq[1] = (seq_t)2;
    a_len = 2;

    /* b sequence – start with 3 */
    b_cap = 8;
    b_seq = (seq_t *)calloc(b_cap, sizeof(seq_t));
    if (!b_seq) abort();
    b_seq[0] = (seq_t)3;
    b_len = 1;

    /* bitset – mark numbers already in a() */
    a_bits = NULL;
    bit_bytes = 0;
    set_bit((seq_t)1);
    set_bit((seq_t)2);
}

/* ------------------------------------------------------------------ */
/*  Main function requested by the specification                      */
seq_t a(idx_t n)
{
    init_data();

    if (n < (idx_t)a_len)               /* value already computed */
        return a_seq[(size_t)n];

    /* compute all terms up to a(n)  */
    for (size_t i = a_len; i <= (size_t)n; ++i)
    {
        /* ---------------------------------------------------------- */
        /*  Compute a(i) using a(i-2) + b(i-2)                          */
        /* ---------------------------------------------------------- */
        seq_t new_a = a_seq[i - 2] + b_seq[i - 2];
        ensure_a_cap(i + 1);
        a_seq[i] = new_a;
        set_bit(new_a);
        a_len = i + 1;

        /* ---------------------------------------------------------- */
        /*  Compute b(i-1) – the next element of the b‑sequence.      */
        /*  This is always required because future calls may need it.  */
        /* ---------------------------------------------------------- */
        /* find the smallest integer > b(i-2) that is either 4
           or not already present in a() */
        seq_t prev_b = b_seq[i - 2];
        seq_t cand = prev_b + 1;
        while (cand != (seq_t)4 && test_bit(cand))
            ++cand;

        /* store the new b value */
        ensure_b_cap(i);
        b_seq[i - 1] = cand;
        b_len = i;   /* now we have b(0)...b(i-1) */
    }

    return a_seq[(size_t)n];
}
