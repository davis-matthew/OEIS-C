/*
 * A247270  Let k == 1 or 5 (mod 6) (A007310). a(n) is the greatest number of
 *          the initial values of k such that k^2+6*n-4 divided by the maximal
 *          possible power of 3 takes only prime values or 1.
 * https://oeis.org/A247270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * a(n) is the largest t such that for the first t numbers
 * k ≡ 1 or 5 (mod 6), the integer (k^2 + 6*n − 4) divided by all
 * powers of 3 is either prime or 1. The function tests the k’s
 * in order until this condition fails and returns the count t.
 */

#include <stdbool.h>
#include <stdint.h>
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
    idx_t k = 1;          /* first k ≡ 1 (mod 6) */
    bool step4 = true;    /* next step is +4 to reach 5 (mod 6) */
    idx_t count = 0;

    for (;;)
    {
        /* Compute k^2 + 6*n - 4 in seq_t */
        seq_t val = (seq_t)k * (seq_t)k + (seq_t)6 * (seq_t)n - 4;

        /* Remove all factors of 3 */
        while ((val % 3) == 0)
            val /= 3;

        /* Accept 1 or a prime */
        if (val == 1 || is_prime_big(val))
        {
            ++count;                      /* this k is good */
            k += step4 ? 4 : 2;           /* advance to next k */
            step4 = !step4;               /* alternate +4, +2 */
        }
        else
            break;                        /* first failure */
    }

    return (seq_t)count;
}
