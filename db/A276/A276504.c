/*
 * A276504  Numbers n such that phi(n) = phi(n+11), with Euler's totient
 *          function phi = A000010.
 * https://oeis.org/A276504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th integer m such that φ(m)=φ(m+11)
 * Range:     u8 n <= 8, u16 n <= 37, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/calc_phi.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t next_candidate = 1;

seq_t a(idx_t n)
{
    while (cache_len < n) {
        while (1) {
            seq_t phi1 = calc_phi((seq_t)next_candidate);
            seq_t phi2 = calc_phi((seq_t)(next_candidate + 11));

            if (phi1 == phi2) {
                seq_t *tmp = (seq_t *)realloc(cache, (cache_len + 1) * sizeof(seq_t));
                if (!tmp && cache_len + 1 > 0) {
                    abort();
                }
                cache = tmp;
                cache[cache_len] = (seq_t)next_candidate;
                cache_len++;
                next_candidate++;
                break;
            }
            next_candidate++;
        }
    }
    return cache[n - 1];
}
