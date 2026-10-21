/*
 * A295055  Solution of the complementary equation a(n) = a(n-2) + b(0) + b(1)
 *          + ... + b(n-1), where a(0) = 1, a(1) = 2, b(0) = 3, and (a(n)) and
 *          (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=2, a(n)=a(n-2)+Σ_{k=0}^{n-1}b(k)   */
/* Complementary sequences a() and b() are increasing and disjoint,          */
/* with initial values a(0)=1, a(1)=2, b(0)=3.                            */
/* The function is memoised and keeps a bit‑set of used numbers.            */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static state for memoisation and bit‑set of used integers. */
static seq_t *a_vals = NULL;          /* array of a[0..max_a]              */
static size_t   a_cap  = 0;           /* allocated size of a_vals          */
static idx_t    max_a  = (idx_t)-1;   /* highest n already computed        */

static seq_t    sumB        = 0;       /* Σ b(0..current-1)                 */
static idx_t    next_candidate = 1;   /* minimal unused integer            */

static uint64_t *used_bits   = NULL;  /* bit‑set for used numbers          */
static size_t   used_cap_words = 0;   /* size of used_bits in 64‑bit words */

static bool     initialized = false;  /* init flag                          */

/* Ensure the bit‑set can address (val). */
static void ensure_bits(idx_t val)
{
    size_t words_needed = (size_t)(val >> 6) + 1;
    if (words_needed > used_cap_words) {
        size_t new_cap = used_cap_words ? used_cap_words << 1 : 64;
        while (new_cap < words_needed)
            new_cap <<= 1;
        uint64_t *new_bits = (uint64_t *)realloc(used_bits, new_cap * sizeof(uint64_t));
        if (!new_bits)
            abort();               /* allocation failed – unlikely at size < 2^64 */
        memset(new_bits + used_cap_words, 0,
               (new_cap - used_cap_words) * sizeof(uint64_t));
        used_bits = new_bits;
        used_cap_words = new_cap;
    }
}

/* Test if integer n is already marked used. */
static inline bool bit_is_set(idx_t n)
{
    size_t w = (size_t)(n >> 6);
    size_t b = (size_t)(n & 63ULL);
    return (used_bits[w] >> b) & 1ULL;
}

/* Mark integer n as used. */
static inline void set_bit(idx_t n)
{
    ensure_bits(n);
    size_t w = (size_t)(n >> 6);
    size_t b = (size_t)(n & 63ULL);
    used_bits[w] |= (1ULL << b);
}

/* Initialise static containers and base values. */
static void init_sequence(void)
{
    /* Allocate a[0] and a[1]. */
    a_cap = 2;
    a_vals = (seq_t *)malloc(a_cap * sizeof(seq_t));
    if (!a_vals)
        abort();
    a_vals[0] = (seq_t)1;
    a_vals[1] = (seq_t)2;
    max_a = 1;

    /* Mark used numbers 1, 2, 3. */
    set_bit((idx_t)1);
    set_bit((idx_t)2);
    set_bit((idx_t)3);

    /* Initialise complementary data. */
    sumB = (seq_t)3;            /* b[0] = 3 */
    next_candidate = 4;         /* next least unused integer */

    initialized = true;
}

/* Main function requested by the problem. */
seq_t a(idx_t n)
{
    if (!initialized)
        init_sequence();

    if (n <= max_a)
        return a_vals[n];

    /* Extend storage for a_vals if needed. */
    if ((size_t)n + 1 > a_cap) {
        size_t new_cap = a_cap ? a_cap << 1 : 8;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        a_vals = tmp;
        a_cap = new_cap;
    }

    /* Compute missing values up to n. */
    for (idx_t i = max_a + 1; i <= n; ++i) {
        /* Find next b[i-1] (the least unused number). */
        while (bit_is_set(next_candidate))
            ++next_candidate;
        seq_t b_val = (seq_t)next_candidate;
        set_bit(next_candidate);        /* mark it used */
        sumB += b_val;                  /* update cumulative sum of bs */

        /* Compute current a[i]. */
        seq_t a_val = a_vals[i - 2] + sumB;
        a_vals[i] = a_val;

        /* Mark the a value as used. */
        set_bit((idx_t)a_val);
    }

    max_a = n;
    return a_vals[n];
}
