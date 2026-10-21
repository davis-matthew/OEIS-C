/*
 * A243812  Let x(0)x(1)... x(q-1)x(q) denote the decimal expansion of a
 *          number n. The sequence lists the numbers such that n and the
 *          number represented by its middle digits x(1)x(2)...x(q-1) have the
 *          same distinct prime divisors.
 * https://oeis.org/A243812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 31, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   append_candidate, digit_len, pow10, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth integer whose distinct prime divisors are equal to
   those of its middle digits (digits 1..d-2). */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/append_candidate.h"
#include "../utils/rad.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n) {
    /* static cache of already found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    /* next integer to test */
    static uint64_t next = 1;

    if (n == 0) return 0;        /* offset is 1, but handle n==0 gracefully */

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        seq_t cur = (seq_t)next;
        int d = digit_len(cur);          /* number of decimal digits of cur */

        if (d >= 3) {                    /* at least 3 digits to have a middle part */
            seq_t pow = pow10(d - 2);     /* 10^(d-2) */
            seq_t mid = (cur / 10) % pow; /* middle digits, leading zeros allowed */

            if (mid != 0) {
                seq_t rad_cur = rad((idx_t)cur);
                seq_t rad_mid = rad((idx_t)mid);
                if (rad_cur == rad_mid) {
                    append_candidate(&cache, &cache_len, &cache_cap, cur);
                }
            }
        }
        next++;                           /* move to the next integer to test */
    }

    return cache[n - 1];                  /* return the nth term (1‑based index) */
}
