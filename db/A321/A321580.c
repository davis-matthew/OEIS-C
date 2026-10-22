/*
 * A321580  Numbers k such that it is possible to reverse a deck of k cards by
 *          a sequence of perfect Faro shuffles with cut.
 * https://oeis.org/A321580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A321580: numbers k such that a deck of k cards can be reversed
 * by a sequence of perfect faro shuffles with cut.
 *
 * The condition is equivalent to: k is 1 or k is even and there exists an
 * integer m ≥ 1 such that 2^m ≡ –1 (mod k+1).  For an odd modulus n this
 * holds iff –1 is a power of 2 modulo n.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* 64‑bit modular multiplication using 128‑bit intermediate */
static uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__int128)a * b % mod);
}

/* test whether -1 is a power of 2 modulo an odd integer n (>1) */
static bool odd_has_power_of_two_neg_one(uint64_t n)
{
    uint64_t target = n - 1;           /* -1 modulo n */
    uint64_t val = 1 % n;              /* 2^0 */

    for (uint64_t i = 0; i < n; ++i) { /* at most n iterations (full cycle) */
        if (val == target)
            return true;
        val = mul_mod64(val, 2, n);
    }
    return false;
}

/* OEIS sequence A321580 */
seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t k = 1;

    while (true) {
        bool good;
        if (k == 1)
            good = true;
        else if ((k & 1ULL) == 0)                /* even k */
            good = odd_has_power_of_two_neg_one(k + 1);
        else
            good = false;                       /* odd k > 1 are never good */

        if (good) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
