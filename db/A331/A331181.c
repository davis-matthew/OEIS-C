/*
 * A331181  Number of values of k, 1 <= k <= n, with A033879(k) = A033879(n),
 *          where A033879(n) is the deficiency of n, 2n-sigma(n).
 * https://oeis.org/A331181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k, 1 <= k <= n, with A033879(k) = A033879(n)
 *            where A033879(n) = deficiency(n) = 2n - sigma(n)
 * Range:     u8 n <= 2433, u16 n <= 9552, u32 n <= 9552, u64 n <= 9552,
 *            u128 n <= 9552, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static idx_t max_n = 0;          /* largest n processed so far                 */
static seq_t *a_array = NULL;    /* a(n) values, 1-indexed                     */
static int64_t *def_keys = NULL; /* distinct deficiency values                */
static seq_t *def_counts = NULL; /* counts of each distinct deficiency value   */
static size_t def_size = 0;      /* number of distinct deficiencies used      */
static size_t def_cap = 0;       /* allocated capacity for deficiency arrays   */

/* Compute the sum of divisors sigma(n).  n <= 10000, so a simple O(sqrt(n)) loop suffices. */
static inline seq_t sigma_of(idx_t n)
{
    uint64_t acc = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            acc += i;
            idx_t j = n / i;
            if (j != i)
                acc += j;
        }
    }
    return (seq_t)acc;
}

static inline void grow_deficiency_arrays(void)
{
    if (def_size == def_cap) {
        size_t new_cap = def_cap ? def_cap * 2 : 4;
        def_keys = (int64_t *)realloc(def_keys, new_cap * sizeof(int64_t));
        def_counts = (seq_t *)realloc(def_counts, new_cap * sizeof(seq_t));
        def_cap = new_cap;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for 0, return 0 for safety */

    /* Ensure we have enough space for a_array */
    if (n > max_n) {
        size_t new_cap = (size_t)n + 1;
        a_array = (seq_t *)realloc(a_array, new_cap * sizeof(seq_t));
        /* Process new indices */
        for (idx_t k = max_n + 1; k <= n; ++k) {
            seq_t sigma = sigma_of(k);
            int64_t deficiency = (int64_t)(2 * k) - (int64_t)sigma;

            /* Search for existing deficiency key */
            size_t i;
            for (i = 0; i < def_size; ++i) {
                if (def_keys[i] == deficiency)
                    break;
            }

            if (i < def_size) {
                /* Existing key */
                def_counts[i] += 1;
                a_array[k] = def_counts[i];
            } else {
                /* New key */
                grow_deficiency_arrays();
                def_keys[def_size] = deficiency;
                def_counts[def_size] = 1;
                a_array[k] = 1;
                def_size += 1;
            }
        }
        max_n = n;
    }
    return a_array[n];
}
