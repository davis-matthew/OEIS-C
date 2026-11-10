/*
 * A305183  Least k such that A048272(k) = -n.
 * https://oeis.org/A305183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 2; a(n) = 4 * A005179(n) for n > 0, where A005179(n) is
 *            the least number with exactly n divisors (computed by a search
 *            over factorizations of n)
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 30, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 43 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool A305183_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

static const uint64_t A305183_primes[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151,
    157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233,
    239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317
};

static bool A305183_fixed;
static seq_t A305183_max;

static seq_t A305183_mul(seq_t x, seq_t y)
{
    if (A305183_fixed) {
        if (x == 0 || y == 0) return 0;
        if (x == A305183_max || y == A305183_max) return A305183_max;
        if (y > A305183_max / x) return A305183_max;
    }
    return x * y;
}

static seq_t A305183_pow(seq_t b, idx_t e)
{
    seq_t r = 1;
    while (e > 0) {
        if (e & 1) r = A305183_mul(r, b);
        e >>= 1;
        if (e > 0) b = A305183_mul(b, b);
    }
    return r;
}

static void A305183_search(idx_t n, int i, idx_t maxd, seq_t cur,
                           seq_t *best, bool *found)
{
    if (n == 1) {
        if (!*found || cur < *best) {
            *best = cur;
            *found = true;
        }
        return;
    }
    idx_t lim = n < maxd ? n : maxd;
    for (idx_t d = 2; d <= lim; d++) {
        if (n % d != 0) continue;
        seq_t v = A305183_mul(cur, A305183_pow((seq_t)A305183_primes[i], d - 1));
        if (*found && v >= *best) continue;
        A305183_search(n / d, i + 1, d, v, best, found);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    A305183_fixed = A305183_fixed_width();
    A305183_max = 0;
    if (A305183_fixed) {
        A305183_max = 0;
        A305183_max = ~A305183_max;
    }
    seq_t best = 0;
    bool found = false;
    A305183_search(n, 0, n, (seq_t)1, &best, &found);
    return best * (seq_t)4;
}
