/*
 * A242667  Number of ways of representing n as the sum of one or more
 *          consecutive squarefree numbers.
 * https://oeis.org/A242667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways of representing n as the sum of one or
 *            more consecutive squarefree numbers
 * Range:     u8 n <= 39, u16 n <= 1580, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static idx_t  max_n = 0;           /* largest number processed so far   */
    static size_t cap   = 0;           /* allocated capacity for arrays     */
    static size_t sz    = 0;           /* counted squarefree numbers        */
    static uint64_t *raw = NULL;       /* squarefree numbers (only for storage) */
    static seq_t  *pref = NULL;       /* prefix sums of squarefree numbers  */

    /* Extend precomputation if a larger n is requested */
    if (n > max_n) {
        idx_t new_max = n;
        size_t needed = (size_t)new_max + 1;        /* include pref[0] */

        if (cap < needed) {
            size_t new_cap = cap ? cap * 2 : needed;
            if (new_cap < needed) new_cap = needed;
            raw = (uint64_t *)realloc(raw, new_cap * sizeof(uint64_t));
            pref = (seq_t *)realloc(pref, new_cap * sizeof(seq_t));
            if (!raw || !pref) exit(1);          /* allocation failed */
            cap = new_cap;
        }

        pref[0] = 0;

        /* Generate squarefree numbers and prefix sums up to new_max */
        for (uint64_t i = max_n + 1; i <= (uint64_t)new_max; ++i) {
            bool sqfree = true;
            uint64_t r = (uint64_t)sqrt((double)i);
            for (uint64_t p = 2; p <= r; ++p) {
                uint64_t pp = p * p;
                if (i % pp == 0) { sqfree = false; break; }
            }
            if (sqfree) {
                raw[sz] = i;
                pref[sz + 1] = pref[sz] + (seq_t)i;
                ++sz;
            }
        }

        max_n = new_max;
    }

    /* Count representations as sums of consecutive squarefree numbers */
    seq_t count = 0;
    for (size_t l = 0; l < sz; ++l) {
        seq_t target = pref[l] + (seq_t)n;
        size_t lo = l + 1, hi = sz;         /* search pref[lo..hi] */
        while (lo <= hi) {
            size_t mid = lo + (hi - lo) / 2;
            seq_t val = pref[mid];
            if (val == target) { ++count; break; }
            else if (val < target) lo = mid + 1;
            else hi = mid - 1;
        }
    }

    return count;
}
