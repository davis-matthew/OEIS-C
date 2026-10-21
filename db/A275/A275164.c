/*
 * A275164  The 167 numbers n such that n(n+1) is 19-smooth.
 * https://oeis.org/A275164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 155, u32 n <= 167, u64 n <= 167,
 *            u128 n <= 167, big any n
 * Helpers:   is19smooth
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * The sequence contains the integers n for which both n and n+1 are 19‑smooth
 * (their prime factors are all ≤ 19).  The terms are enumerated in increasing
 * order.  They are determined by a simple brute‑force search of consecutive
 * smooth numbers.  This implementation computes all 167 terms on first call
 * and then answers each query by table lookup.
 */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is19smooth.h"

seq_t a(idx_t n)
{
    /* static cache for the 167 terms */
    static seq_t cache[167];
    static bool initialized = false;
    static idx_t count = 0;

    if (!initialized) {
        idx_t m = 1;
        /* find the first 167 terms */
        while (count < 167) {
            if (is19smooth((uint64_t)m) && is19smooth((uint64_t)(m + 1))) {
                cache[count++] = (seq_t)m;
            }
            ++m;
        }
        initialized = true;
    }

    if (n == 0) return 0;           /* undefined before offset */
    if (n <= count) return cache[n - 1];
    return 0;                        /* undefined beyond known range */
}
