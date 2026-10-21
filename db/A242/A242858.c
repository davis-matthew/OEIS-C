/*
 * A242858  E.g.f. ((e^x-1)^2*e^x) / (2*(1-(e^x-1)^3)).
 * https://oeis.org/A242858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = * 1/2 * sum_{m=2..n, m%3==0 or 2} m! * Stirling2(n,m) * *
 *            We compute the values by maintaining the array * T[m] = m! *
 *            Stirling2(current_n, m) for the current n. * For each increment
 *            n → n+1 we use the recurrence * T_{n+1}[m] = m * ( T_n[m] +
 *            T_n[m-1] ), m ≥ 1 * and set T_{n+1}[0] = 0. * T...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Static storage for values and cache */
static seq_t *T = NULL;          /* T[m] = m! * S(n, m) for current n */
static seq_t *a_cache = NULL;    /* cached a(n) values */
static idx_t max_n = (idx_t)(-1);/* highest n that has been computed */
static size_t capacity = 0;      /* allocated size of T and a_cache */

/* Resize the dynamic arrays to at least n+1 elements */
static void resize_to(idx_t n)
{
    if (capacity > (size_t)n)
        return;

    size_t new_cap = (size_t)n + 1;
    seq_t *new_T = alloc_seq_array(new_cap);
    seq_t *new_a = alloc_seq_array(new_cap);

    if (T)
        memcpy(new_T, T, capacity * sizeof(seq_t));
    if (a_cache)
        memcpy(new_a, a_cache, capacity * sizeof(seq_t));

    /* initialise any new entries to zero (already zero for <capacity) */
    for (size_t i = capacity; i < new_cap; ++i) {
        new_T[i] = 0;
        new_a[i] = 0;
    }

    if (T)      free_seq_array(T);
    if (a_cache)free_seq_array(a_cache);

    T = new_T;
    a_cache = new_a;
    capacity = new_cap;
}

/* Ensure the arrays are initialised for n = 0 */
static void init_first(void)
{
    if (max_n != (idx_t)(-1))
        return;   /* already initialised */

    resize_to(0);
    T[0] = (seq_t)1;    /* 0! * S(0,0) = 1 */
    a_cache[0] = (seq_t)0;
    max_n = 0;
}

seq_t a(idx_t n)
{
    init_first();

    /* Compute forward from max_n + 1 up to n */
    if (n > max_n) {
        resize_to(n);

        for (idx_t cur = max_n + 1; cur <= n; ++cur) {
            /* Update T[m] for the new n in descending order */
            /* T[cur] (i.e. T[n]) is currently 0 after resizing */
            for (idx_t m = cur; m > 0; --m) {
                seq_t sum = T[m] + T[m - 1];
                T[m] = sum * (seq_t)m;
            }
            T[0] = (seq_t)0;   /* S(cur,0) = 0 for cur>0 */

            /* Compute a(cur) = sum_{m=2..cur, m%3==0 or 2} T[m] / 2 */
            seq_t sum = (seq_t)0;
            for (idx_t m = 2; m <= cur; ++m) {
                if (m % 3 == 0 || m % 3 == 2) {
                    sum += T[m];
                }
            }
            a_cache[cur] = sum / (seq_t)2;
        }
        max_n = n;
    }

    return a_cache[n];
}
