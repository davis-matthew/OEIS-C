/*
 * A397710  Composite numbers k such that k = rad(k) * lpf(k), where rad =
 *          A007947 and lpf is the least prime factor of k.
 * https://oeis.org/A397710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   lpf, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Incremental search for OEIS A397710: composite k with 
   k = rad(k) * lpf(k).  Values are memoised so any order of calls
   is handled in O(number of terms found). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/rad.h"   /* seq_t rad(idx_t m); */
#include "../utils/lpf.h"   /* idx_t lpf(idx_t n); */
#if defined(__cplusplus)
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard anyway */

    /* state used for memoisation */
    static idx_t next_k = 4;       /* next integer to test (k ≥ 4) */

#if defined(__cplusplus)
    static std::vector<seq_t> values; /* a(1) stored at values[0] */
    if (values.size() < (size_t)n) {
        values.reserve(n);                     /* avoid many resizes */
        while (values.size() < (size_t)n) {
            seq_t k_val = (seq_t)next_k;
            idx_t lpf_val = lpf(next_k);
            seq_t rad_val = rad((idx_t)k_val);
            if (rad_val * (seq_t)lpf_val == k_val) {
                values.push_back(k_val);
            }
            ++next_k;
        }
    }
    return values[n - 1];
#else
    static seq_t *values = NULL;
    static size_t values_len = 0, values_cap = 0;
    if (values_cap == 0) {
        values_cap = 256;
        values = (seq_t *)malloc(values_cap * sizeof(seq_t));
        if (!values) return 0;               /* allocation failure */
    }
    while (values_len < (size_t)n) {
        seq_t k_val = (seq_t)next_k;
        idx_t lpf_val = lpf(next_k);
        seq_t rad_val = rad((idx_t)k_val);
        if (rad_val * (seq_t)lpf_val == k_val) {
            if (values_len >= values_cap) {
                values_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(values, values_cap * sizeof(seq_t));
                if (!tmp) { free(values); return 0; }
                values = tmp;
            }
            values[values_len++] = k_val;
        }
        ++next_k;
    }
    return values[n - 1];
#endif
}
