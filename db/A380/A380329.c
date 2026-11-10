/*
 * A380329  a(n) = A151800((n-1)*A151800(n)).
 * https://oeis.org/A380329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A151800((n-1)*A151800(n)), A151800(n) = least prime > n;
 *            a(0)=2
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9796 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

static bool A151800_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool A151800_is_prime_big(seq_t n) {
    if (n < 2) return false;
    if (A151800_fits_u64(n)) return is_prime_u64((uint64_t)n);
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

/* least prime > x */
static seq_t A151800_next(seq_t x)
{
    seq_t m = x + 1;
    if (m <= 2) return 2;
    if ((m & 1) == 0) m += 1;
    while (!A151800_is_prime_big(m))
        m += 2;
    return m;
}

seq_t a(idx_t n)
{
    if (n == 0) return 2;
    seq_t p = A151800_next((seq_t)n);
    seq_t arg = ((seq_t)n - 1) * p;
    return A151800_next(arg);
}
