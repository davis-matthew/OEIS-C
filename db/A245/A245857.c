/*
 * A245857  Number of preferential arrangements of n labeled elements such
 *          that the minimal number of elements per rank equals 4.
 * https://oeis.org/A245857
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A232475(n) - A245790(n) where A232475(n) counts
 *            preferential arrangements of n labeled elements with all blocks
 *            size ≥ 4, and A245790(n) counts those with all blocks size ≥ 5
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 17, u64 n <= 26, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* POSIX memory allocation functions are used for the C build.
   For the C++ build, std::vector provides automatic
   construction and reallocation of std::big_integer (or other)
   arbitrary‑precision types. */
#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>

#ifdef __cplusplus
using SeqContainer = std::vector<seq_t>;
static SeqContainer f4_container;
static SeqContainer f5_container;
#else
/* In C, use raw pointers and realloc.  The cast to (seq_t*) is needed
   because realloc returns void*.  The added loop zero‑initializes new
   elements so that the type 'seq_t' is treated as a POD.  For an
   arbitrary‑precision std::big_integer on a C++ build, the C path will
   never be taken. */
static seq_t *f4_ptr = NULL;
static seq_t *f5_ptr = NULL;
#endif

static idx_t _max_n = 0;

/* Ensure that the container has room for indices up to n. */
static void _ensure_capacity(idx_t n)
{
#ifdef __cplusplus
    if (n < _max_n) return;
    if (f4_container.size() < n + 1) {
        f4_container.resize(n + 1);
        f5_container.resize(n + 1);
    }
#else
    if (n <= _max_n) return;

    seq_t *tmp4 = (seq_t *)realloc(f4_ptr, (size_t)(n + 1) * sizeof(seq_t));
    seq_t *tmp5 = (seq_t *)realloc(f5_ptr, (size_t)(n + 1) * sizeof(seq_t));
    if (!tmp4 || !tmp5) {
        /* Allocation failed – terminate. */
        exit(1);
    }
    /* Zero‑initialize any new entries. */
    for (idx_t i = _max_n + 1; i <= n; ++i) {
        tmp4[i] = 0;
        tmp5[i] = 0;
    }
    f4_ptr = tmp4;
    f5_ptr = tmp5;
#endif
}

/* Compute the two auxiliary sequences up to index n. */
static void _compute_up_to(idx_t n)
{
    if (n <= _max_n) return;
    _ensure_capacity(n);

#ifdef __cplusplus
    /* Base cases (index 0) are automatically set to 1 by vec resize. */
    if (_max_n == 0) {
        f4_container[0] = 1;
        f5_container[0] = 1;
    }
    for (idx_t i = _max_n + 1; i <= n; ++i) {
        /* Compute f4[i] : all blocks size >= 4 */
        seq_t sum4 = 0;
        if (i >= 4) {
            seq_t comb = 1;
            for (idx_t k = 0; k < 4; ++k)
                comb = comb * (seq_t)(i - k) / (seq_t)(k + 1);
            for (idx_t t = 4; t <= i; ++t) {
                sum4 += comb * f4_container[i - t];
                if (t < i)
                    comb = comb * (seq_t)(i - t) /
                           (seq_t)(t + 1);
            }
        }
        f4_container[i] = sum4;

        /* Compute f5[i] : all blocks size >= 5 */
        seq_t sum5 = 0;
        if (i >= 5) {
            seq_t comb = 1;
            for (idx_t k = 0; k < 5; ++k)
                comb = comb * (seq_t)(i - k) / (seq_t)(k + 1);
            for (idx_t t = 5; t <= i; ++t) {
                sum5 += comb * f5_container[i - t];
                if (t < i)
                    comb = comb * (seq_t)(i - t) /
                           (seq_t)(t + 1);
            }
        }
        f5_container[i] = sum5;
    }
    _max_n = n;
#else
    if (_max_n == 0) {
        f4_ptr[0] = 1;
        f5_ptr[0] = 1;
    }
    for (idx_t i = _max_n + 1; i <= n; ++i) {
        seq_t sum4 = 0;
        if (i >= 4) {
            seq_t comb = 1;
            for (idx_t k = 0; k < 4; ++k)
                comb = comb * (seq_t)(i - k) / (seq_t)(k + 1);
            for (idx_t t = 4; t <= i; ++t) {
                sum4 += comb * f4_ptr[i - t];
                if (t < i)
                    comb = comb * (seq_t)(i - t) /
                           (seq_t)(t + 1);
            }
        }
        f4_ptr[i] = sum4;

        seq_t sum5 = 0;
        if (i >= 5) {
            seq_t comb = 1;
            for (idx_t k = 0; k < 5; ++k)
                comb = comb * (seq_t)(i - k) / (seq_t)(k + 1);
            for (idx_t t = 5; t <= i; ++t) {
                sum5 += comb * f5_ptr[i - t];
                if (t < i)
                    comb = comb * (seq_t)(i - t) /
                           (seq_t)(t + 1);
            }
        }
        f5_ptr[i] = sum5;
    }
    _max_n = n;
#endif
}

/* Public API required by the framework */
seq_t a(idx_t n)
{
    if (n < 4) return 0;          /* offset at 4 */
    _compute_up_to(n);
#ifdef __cplusplus
    return f4_container[n] - f5_container[n];
#else
    return f4_ptr[n] - f5_ptr[n];
#endif
}
