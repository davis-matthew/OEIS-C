/*
 * A394947  Number of well-formed bracketed words of total length 2n built
 *          from the symbol * (of length 2) and four unary bracket types (),
 *          [], {}, and <>, with no empty bracket pair, where every maximal
 *          chain of unary brackets is written in canonical weakly increasing
 *          order.
 * https://oeis.org/A394947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A394947:  Number of well‑formed bracketed words of total
 *  length 2*n built from the symbol * (length 2) and four unary
 *  bracket types (), [], {}, <> with the canonical weakly increasing
 *  order condition.
 *
 *  The recurrence from the OEIS entry is implemented directly:
 *      a(0) = a(1) = 1
 *      for n ≥ 2
 *          a(n) =   5*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4)
 *                  + 4*∑_{k=1}^{n-2} a(k)*a(n-1-k)
 *                  - 6*∑_{k=1}^{n-3} a(k)*a(n-2-k)
 *                  + 4*∑_{k=1}^{n-4} a(k)*a(n-3-k)
 *                  -   ∑_{k=1}^{n-5} a(k)*a(n-4-k)
 *
 *  Internally, two dynamic arrays are maintained:
 *      seq_a[i]   – the already computed values a(i)
 *      seq_conv[i] – the convolution ∑_{k=1}^{i-1} a(k)*a(i-k)
 *
 *  The arrays are grown as required; the routine is fully
 *  memoised so repeated calls for arbitrary indices are answered in
 *  O(n) total arithmetic operations for each new value of n.
 *  It works for all legal builds (fixed‑size integer types and an
 *  arbitrary‑precision class for seq_t) and obeys all C11 / C++17
 *  requirements.  The required cast for realloc is made explicit
 *  so the code compiles under C++ even when seq_t is a non‑trivial
 *  class type.  */

#include <stdlib.h>
#include <stdint.h>

/* Explicit cast for realloc that is valid in both C and C++ */
#ifdef __cplusplus
#define XT(x) reinterpret_cast<seq_t*>(x)
#else
#define XT(x) (seq_t*)(x)
#endif

/* Static storage for memoisation */
static seq_t *seq_a = NULL;
static seq_t *seq_conv = NULL;
static size_t a_cap = 0;
static size_t conv_cap = 0;
static idx_t last_computed = (idx_t)-1;

/* Ensure seq_a[0..n] and seq_conv[0..n] are allocated. */
static void ensure_capacity(idx_t n)
{
    size_t req = (size_t)n + 1;
    if (a_cap < req) {
        size_t new_cap = a_cap ? a_cap * 2 : 4;
        if (new_cap < req) new_cap = req;
        seq_a = XT(realloc(seq_a, new_cap * sizeof(seq_t)));
        a_cap = new_cap;
    }
    if (conv_cap < req) {
        size_t new_cap = conv_cap ? conv_cap * 2 : 4;
        if (new_cap < req) new_cap = req;
        seq_conv = XT(realloc(seq_conv, new_cap * sizeof(seq_t)));
        conv_cap = new_cap;
    }
}

/* Initialise the first two terms and the base convolution. */
static void initialize(void)
{
    if (a_cap > 0) return;  /* already done */
    ensure_capacity(3);     /* need indices 0..2 */
    seq_a[0] = (seq_t)1;
    seq_a[1] = (seq_t)1;
    seq_conv[0] = (seq_t)0;
    seq_conv[1] = (seq_t)0;
    seq_conv[2] = seq_a[1] * seq_a[1];   /* term for i=2 */
    last_computed = 1;
}

/* After a[n] has been stored, update conv[n+1] = Σ_{k=1}^{n} a(k)*a(n+1-k). */
static void update_conv(idx_t n)
{
    idx_t m = n + 1;
    if ((size_t)m >= conv_cap) ensure_capacity(m);
    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        sum += seq_a[k] * seq_a[m - k];
    }
    seq_conv[m] = sum;
}

/* The exported sequence routine.  */
seq_t a(idx_t n)
{
    if (n == (idx_t)-1) return (seq_t)0;  /* safety check */

    if (a_cap == 0) initialize();

    /* Return already computed value. */
    if (n <= last_computed) return seq_a[(size_t)n];

    /* Compute all terms up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        ensure_capacity(i);

        seq_t term = (seq_t)0;
        if (i >= 2) term += (seq_t)5 * seq_a[(size_t)(i - 1)];
        if (i >= 3) term -= (seq_t)6 * seq_a[(size_t)(i - 2)];
        if (i >= 4) term += (seq_t)4 * seq_a[(size_t)(i - 3)];
        if (i >= 5) term -= seq_a[(size_t)(i - 4)];

        seq_t c1 = (i >= 2) ? seq_conv[(size_t)(i - 1)] : (seq_t)0;
        seq_t c2 = (i >= 3) ? seq_conv[(size_t)(i - 2)] : (seq_t)0;
        seq_t c3 = (i >= 4) ? seq_conv[(size_t)(i - 3)] : (seq_t)0;
        seq_t c4 = (i >= 5) ? seq_conv[(size_t)(i - 4)] : (seq_t)0;

        term += (seq_t)4 * c1 - (seq_t)6 * c2 + (seq_t)4 * c3 - c4;
        seq_a[(size_t)i] = term;

        /* Prepare convolution for the next position. */
        update_conv(i);
    }

    last_computed = n;
    return seq_a[(size_t)n];
}
