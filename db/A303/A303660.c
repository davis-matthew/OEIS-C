/*
 * A303660  Number of ways to write 2*n+1 as p + 3^k + 5^m, where p is a
 *          prime, and k and m are nonnegative integers.
 * https://oeis.org/A303660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of 2*n+1 as p + 3^k + 5^m,
 *            where p is prime, k and m are non‑negative integers
 * Range:     u8 n <= 498, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 8375 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
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
    if (n < 1) return 0;                  /* offset starts at 1 */

    /* target odd number 2*n+1 */
    seq_t target = ((seq_t)n) * 2 + 1;

    seq_t count = 0;

    seq_t pow3 = 1;
    /* iterate over all k such that 3^k <= target‑2 */
    while (pow3 <= target - 2u) {
        /* maximum value that 5^m can take for this k */
        seq_t bound = target - pow3 - 2u;

        seq_t pow5 = 1;
        /* iterate over all m such that 5^m <= bound */
        while (pow5 <= bound) {
            seq_t p = target - pow3 - pow5;
            if (p >= 2u && is_prime_big(p))
                ++count;

            /* advance to the next power of 5, avoid overflow */
            if (pow5 > bound / 5u) break;
            pow5 *= 5u;
        }

        /* advance to the next power of 3, avoid overflow */
        if (pow3 > (target - 2u) / 3u) break;
        pow3 *= 3u;
    }

    return count;
}
