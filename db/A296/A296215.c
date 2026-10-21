/*
 * A296215  Solution of the complementary equation a(n) = a(1)*b(n-2) +
 *          a(2)*b(n-3) + ... + a(n-1)*b(0), where a(0) = 1, a(1) = 3, b(0) =
 *          2, and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1}^{n-1} a(i)*b(n-1-i) with a(0)=1, a(1)=3, b(0)=2
 *            and a,b are increasing complementary sequences
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>           /* size_t */
#include "../utils/grow_seq.h" /* grow_seq() */

static seq_t *a_cache = NULL;  /* cached values of a() */
static size_t a_cap  = 0;      /* current capacity for a_cache   */
static size_t a_len  = 0;      /* number of values stored in a_cache    */

static seq_t *b_cache = NULL;  /* cached values of b() */
static size_t b_cap  = 0;      /* current capacity for b_cache   */
static size_t b_len  = 0;      /* number of values stored in b_cache    */

static idx_t cand = 3;         /* next integer to test for b() (first unused candidate) */
static size_t a_idx_for_cand = 0; /* index in a_cache used for membership check */

/* Ensure a_cache has capacity of at least cap elements. */
static void ensure_a_cap(size_t cap)
{
    if (cap > a_cap)
        grow_seq(&a_cache, &a_cap, cap);
}

/* Ensure b_cache has capacity of at least cap elements. */
static void ensure_b_cap(size_t cap)
{
    if (cap > b_cap)
        grow_seq(&b_cache, &b_cap, cap);
}

/* Return the next b() value that is NOT in a_cache() and NOT already
   generated for b().  cand always points to the smallest unused integer. */
static seq_t next_b_value(void)
{
    /* Skip integers that are in a_cache() */
    while (a_idx_for_cand < a_len &&
           a_cache[a_idx_for_cand] < (seq_t)cand)
        ++a_idx_for_cand;

    if (a_idx_for_cand < a_len &&
        a_cache[a_idx_for_cand] == (seq_t)cand) {
        ++cand;
        return next_b_value();
    }

    /* cand is not in a_cache(); it becomes the next b() value. */
    seq_t value = (seq_t)cand;
    ++cand;         /* advance for the next call */
    return value;
}

seq_t a(idx_t n)
{
    /* handle base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* Initialize caches on first use. */
    if (a_len == 0) {
        ensure_a_cap(2);
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)3;
        a_len = 2;
    }

    if (b_len == 0) {
        ensure_b_cap(1);
        b_cache[0] = (seq_t)2;
        b_len = 1;
        /* next candidate after 2 must start at 3 */
        cand = 3;
    }

    /* Make sure we have space for a[n] and any b values needed. */
    ensure_a_cap((size_t)n + 1);

    /* Compute any missing a-values up to n. */
    for (idx_t k = a_len; k <= n; ++k) {
        /* Ensure b[0 .. k-2] have been generated. */
        while (b_len <= (size_t)(k - 2)) {
            ensure_b_cap(b_len + 1);
            b_cache[b_len++] = next_b_value();
        }

        /* Compute a(k) = Σ_{i=1}^{k-1} a(i) * b(k-1-i) */
        seq_t sum = 0;
        for (idx_t i = 1; i < k; ++i)
            sum += a_cache[i] * b_cache[(size_t)(k - 1 - i)];
        a_cache[k] = sum;
    }

    a_len = (size_t)n + 1;
    return a_cache[(size_t)n];
}
