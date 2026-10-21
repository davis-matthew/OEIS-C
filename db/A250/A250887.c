/*
 * A250887  G.f. A(x) satisfies: x = A(x) * (1 + A(x)) * (1 - 3*A(x)).
 * https://oeis.org/A250887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   x = A(x) * (1 + A(x)) * (1 - 3*A(x)) a(n) = 2*Σ_{k=1}^{n-1}
 *            a(k)*a(n-k) + 3*Σ_{k=1}^{n-1} a(k)*Σ_{i=1}^{n-k-1} a(i)*a(n-k-i)
 *            (for n ≥ 2; a(1)=1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */
#ifdef __cplusplus
#include <algorithm>  /* std::copy */
#endif

/* Persistent cache for the sequence and its pairwise convolution */
static seq_t *a_seq     = NULL;   /* a[0..]  (a[0] unused) */
static seq_t *conv2_seq = NULL;   /* convolution a*a  */
static size_t cache_cap = 0;      /* capacity of the two arrays (same for both) */
static idx_t  max_computed = 0;   /* largest index whose value is ready */

/* Resize the two arrays to at least 'needed' elements (including index 0).
   After resizing, all newly allocated elements are set to zero.  */
static void
ensure_capacity(idx_t needed)
{
    if (needed <= (idx_t)cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < (size_t)needed)
        new_cap <<= 1;          /* grow geometrically */

    seq_t *old_a     = a_seq;
    seq_t *old_conv2 = conv2_seq;

#ifdef __cplusplus
    /* C++: use new[]/delete[] to construct objects */
    a_seq     = new seq_t[new_cap];
    conv2_seq = new seq_t[new_cap];
    /* copy old data into the new block */
    if (old_a)
        std::copy(old_a,     old_a   + cache_cap, a_seq);
    if (old_conv2)
        std::copy(old_conv2, old_conv2 + cache_cap, conv2_seq);
    /* free old storage */
    if (old_a)
        delete[] old_a;
    if (old_conv2)
        delete[] old_conv2;
#else
    /* C: use malloc/realloc */
    a_seq     = (seq_t *)realloc(a_seq,     new_cap * sizeof(seq_t));
    conv2_seq = (seq_t *)realloc(conv2_seq, new_cap * sizeof(seq_t));
#endif

    /* zero the newly allocated tail */
    for (size_t i = cache_cap; i < new_cap; ++i) {
        a_seq[i]     = 0;
        conv2_seq[i] = 0;
    }

    cache_cap = new_cap;
}

/* Make sure indexes up to n (>=1) have been computed. */
static void
ensure_storage(idx_t n)
{
    if (max_computed >= n)
        return;

    ensure_capacity((idx_t)cache_cap < n+1 ? n+1 : (idx_t)cache_cap);

    /* first element (a[1]) has to be seeded once */
    if (max_computed == 0) {
        a_seq[1]     = 1;
        conv2_seq[1] = 0;
        max_computed = 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum2 = 0;
        seq_t sum3 = 0;
        for (idx_t k = 1; k < i; ++k) {
            sum2 += a_seq[k] * a_seq[i - k];
            sum3 += a_seq[k] * conv2_seq[i - k];
        }
        a_seq[i] = (seq_t)2 * sum2 + (seq_t)3 * sum3;

        /* conv2_seq[i] = Σ_{k=1}^{i-1} a(k)*a(i-k) */
        seq_t prod_sum = 0;
        for (idx_t k = 1; k < i; ++k)
            prod_sum += a_seq[k] * a_seq[i - k];
        conv2_seq[i] = prod_sum;
    }

    max_computed = n;
}

/* External function providing the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* safety for n==0, offset is 1 */
    ensure_storage(n);
    return a_seq[n];
}
