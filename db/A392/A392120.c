/*
 * A392120  Numbers k such that (k^3 - greatest prime < k^3) < (-k^3 + least
 *          prime > k^3).
 * https://oeis.org/A392120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A392120
    a(n) = the nth integer k ≥ 1 such that
           (k³ – greatest prime < k³) < (‑k³ + least prime > k³)
    Equivalently, let P₋(x) be the largest prime < x and
                      P₊(x) the smallest prime > x;
    then a(n) is the nth k satisfying  P₋(k³) + P₊(k³)  >  2·k³ .
    Offset : 1
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

static seq_t prev_prime(seq_t x)
/* return the largest prime < x; 0 if none */
{
    if (x <= (seq_t)2)
        return (seq_t)0;
    seq_t p = x - (seq_t)1;
    while (!is_prime_big(p))
        p -= (seq_t)1;
    return p;
}

static seq_t next_prime(seq_t x)
/* return the smallest prime  > x */
{
    seq_t p = x + (seq_t)1;
    while (!is_prime_big(p))
        p += (seq_t)1;
    return p;
}

seq_t a(idx_t n)
/* Sequence A392120: nth term (offset 1) */
{
    if (n == 0)   /* offset 1 – undefined for n = 0 */
        return (seq_t)0;

    idx_t found = 0;   /* how many terms found so far     */
    seq_t k = (seq_t)1;   /* candidate k, start at 1         */

    while (found < n)
    {
        /* cube of k */
        seq_t cube = k * k * k;

        /* nearest primes around k³ */
        seq_t low  = prev_prime(cube);   /* largest prime < k³ */
        seq_t high = next_prime(cube);   /* smallest prime > k³ */

        /* test the condition P₋ + P₊  >  2·k³ */
        if (low + high > (seq_t)2 * cube)
        {
            ++found;
            if (found == n)
                return k;
        }
        ++k;
    }

    /* unreachable – just to keep the compiler happy */
    return (seq_t)0;
}
