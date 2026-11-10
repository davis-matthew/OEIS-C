/*
 * A247902  First differences of A247683.
 * https://oeis.org/A247902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A247683(n+1) - A247683(n), where A247683 lists the odd
 *            composites congruent to 7 mod 9 (found by a search over 18k+7)
 * Range:     u8 n <= 5, u16 n <= 6725, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9866 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

static bool A247902_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool A247902_is_prime(seq_t n) {
    if (n < 2) return false;
    if (A247902_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t b = (seq_t)base_list[i];
        if (b >= n) continue;

        seq_t x = 1;
        seq_t pw = b % n;
        seq_t ex = d;
        while (ex > 0) {
            if (ex & 1) x = (x * pw) % n;
            pw = (pw * pw) % n;
            ex >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t cand = 7;
    seq_t prev = 0;

    while (1) {
        if (!A247902_is_prime(cand)) {
            ++found;
            if (found == n) prev = cand;
            else if (found == n + 1) return cand - prev;
        }
        cand += 18;
    }
}
