/*
 * A250890  G.f. A(x) satisfies: x = A(x) * (1 + 2*A(x)) * (1 - 5*A(x)).
 * https://oeis.org/A250890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The generating function of A250890 satisfies x = A(x) * (1 +
 *            2*A(x)) * (1 - 5*A(x)) which is equivalent to the Lagrange
 *            inversion formula giving the recurrence (for n ≥ 2): a(n) = 3 ·
 *            Σ_{k=1}^{n-1} a(k) · a(n-k) + 10 · Σ_{i+j+k=n, i,j,k≥1} a(i) ·
 *            a(j) · a(k) Initial value: a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
/* C++ builds use std::vector for safe construction of seq_t objects. */
#include <vector>
#endif

static seq_t *a_cache = NULL;   /* a[n] */
static seq_t *c_cache = NULL;   /* convolution a*a */
static seq_t *d_cache = NULL;   /* convolution a*a*a */
static size_t cache_len = 0;    /* allocated length (indices 0 … cache_len-1) */
static idx_t max_computed = 0;  /* greatest n for which a[n] is already known */

static void ensure_cache(idx_t n)
{
#ifdef __cplusplus
    /* C++: use std::vector for safe resize and zero‑initialisation. */
    static std::vector<seq_t> a_vec;
    static std::vector<seq_t> c_vec;
    static std::vector<seq_t> d_vec;

    if (n + 1 > a_vec.size()) {
        a_vec.resize(n + 1);
        c_vec.resize(n + 1);
        d_vec.resize(n + 1);
    }
    a_cache = a_vec.data();
    c_cache = c_vec.data();
    d_cache = d_vec.data();
    cache_len = a_vec.size();
#else
    /* C: use realloc with explicit casting; seq_t is a plain integral type. */
    if (cache_len <= (size_t)n) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_a = (seq_t*)realloc(a_cache, new_len * sizeof(seq_t));
        seq_t *new_c = (seq_t*)realloc(c_cache, new_len * sizeof(seq_t));
        seq_t *new_d = (seq_t*)realloc(d_cache, new_len * sizeof(seq_t));
        if (!new_a || !new_c || !new_d) {
            /* abort on allocation failure – unlikely in test harness */
            exit(1);
        }
        /* Zero the freshly allocated tail */
        for (size_t i = cache_len; i < new_len; ++i) {
            new_a[i] = 0;
            new_c[i] = 0;
            new_d[i] = 0;
        }
        a_cache = new_a;
        c_cache = new_c;
        d_cache = new_d;
        cache_len = new_len;
    }
#endif
}

/* Main routine: return the n‑th term (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n < 1)  /* undefined: return 0 for safety */
        return (seq_t)0;

    ensure_cache(n);

    /* If the value is already known, simply return it */
    if (n <= max_computed)
        return a_cache[n];

    /* Compute terms incrementally from max_computed+1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1) {
            a_cache[1] = (seq_t)1;
            /* c_cache[1] and d_cache[1] stay zero; the recurrence for i=1
               would produce zero anyway. */
            continue;
        }

        /* compute c_i = Σ_{k=1}^{i-1} a(k) * a(i-k) */
        seq_t sum_c = 0;
        for (idx_t k = 1; k < i; ++k) {
            sum_c += a_cache[k] * a_cache[i - k];
        }
        c_cache[i] = sum_c;

        /* compute d_i = Σ_{j=1}^{i-1} c(j) * a(i-j) */
        seq_t sum_d = 0;
        for (idx_t j = 1; j < i; ++j) {
            sum_d += c_cache[j] * a_cache[i - j];
        }
        d_cache[i] = sum_d;

        /* recurrence: a_i = 3 * c_i + 10 * d_i */
        a_cache[i] = (seq_t)3 * sum_c + (seq_t)10 * sum_d;
    }

    max_computed = n;
    return a_cache[n];
}
