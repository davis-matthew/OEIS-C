/*
 * A305232  Number of ordered ways to write 2*n+1 as p + binomial(2k,k) +
 *          2*binomial(2m,m), where p is an odd prime, and k and m are
 *          nonnegative integers.
 * https://oeis.org/A305232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered representations of 2*n+1 as p +
 *            choose(2k,k) + 2*choose(2m,m), where p is an odd prime, and k,m
 *            are non‑negative integers
 * Range:     u8 n <= 1142, u16 n <= 99999, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 106 terms through n = 9914 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
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

static seq_t central_binomial(idx_t k, seq_t limit)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* multiply then divide – division is exact each step */
        res = res * (k + i) / i;
        if (res > limit)  /* no need to compute further */
            return limit + 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t target = (seq_t)n * 2 + 1;          /* 2*n + 1 */
    seq_t count  = 0;

    for (idx_t k = 0; ; ++k)
    {
        seq_t cbk = central_binomial(k, target);
        if (cbk > target)
            break;                           /* further k will be larger */

        for (idx_t m = 0; ; ++m)
        {
            seq_t cbm = central_binomial(m, target);
            if (cbm > target)
                break;                       /* further m will be larger */

            /* term <= 1 iff cbk + 2 cbm + 1 >= target; checked first, since
               the subtraction wrapped around in an unsigned seq_t */
            if (cbk + cbm * 2 + 1 >= target)
                continue;
            seq_t term = target - cbk - (cbm * 2);
            if (((term & 1) == 1) && is_prime_big(term))
                ++count;
        }
    }

    return count;
}
