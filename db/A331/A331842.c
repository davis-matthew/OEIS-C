/*
 * A331842  Number of positive integer solutions (x,y) to the equation x^y =
 *          y^(nx).
 * https://oeis.org/A331842
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positive integer solutions (x,y) of x^y = y^(n
 *            x)
 * Range:     u8 n <= 16384, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 99 terms through n = 9611 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* test whether val is an exact k‑th power,
   if so store the root in *root_out and return true */
static bool is_kth_power(uint64_t val, idx_t k, uint64_t *root_out)
{
    if (k == 0)
        return false;
    uint64_t a = 2;
    for (;;) {
        uint64_t prod = 1;
        idx_t i;
        for (i = 0; i < k; ++i) {
            if (prod > val / a) {          /* would overflow or exceed val */
                prod = val + 1;
                break;
            }
            prod *= a;
        }
        if (prod == val) {
            *root_out = a;
            return true;
        }
        if (prod > val)
            break;
        ++a;
    }
    return false;
}

/* check if a is a perfect power (>1) */
static bool is_power_of_integer(uint64_t a)
{
    if (a <= 1)
        return false;
    for (idx_t r = 2; ; ++r) {
        if (r >= 64)          /* shift of 64 or more UB */
            break;
        uint64_t limit = 1ULL << r;
        if (limit > a)
            break;
        uint64_t root;
        if (is_kth_power(a, r, &root))
            return true;
    }
    return false;
}

/* integer square root, works for the small values we need */
static idx_t isqrt_u64(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* behaviour below offset is irrelevant */

    seq_t count = 1;                  /* (1,1) */

    for (idx_t k = 1; ; ++k) {
        uint64_t two_pow = 1ULL << k;
        uint64_t bound   = (uint64_t)n * (1 + k);
        if (two_pow > bound)
            break;                    /* no more k can give a²ⁿ ≤ n(1+k) */

        idx_t nk = n * k;
        idx_t sq = isqrt_u64(nk);
        idx_t d;
        for (d = 1; d <= sq; ++d) {
            if (nk % d == 0) {
                /* first divisor t = d */
                idx_t t = d;
                uint64_t rhs = (uint64_t)n + (uint64_t)t;
                uint64_t a_val;
                if (is_kth_power(rhs, k, &a_val)) {
                    if (!is_power_of_integer(a_val))
                        count++;
                }

                /* second divisor t = nk/d if distinct */
                idx_t d2 = nk / d;
                if (d2 != d) {
                    t = d2;
                    rhs = (uint64_t)n + (uint64_t)t;
                    if (is_kth_power(rhs, k, &a_val)) {
                        if (!is_power_of_integer(a_val))
                            count++;
                    }
                }
            }
        }
    }
    return count;
}
