/*
 * A273283  Least prime not less than the geometric mean of all prime divisors
 *          of n counted with multiplicity.
 * https://oeis.org/A273283
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime ≥ n^(1/ω(n)), where ω(n) is the number of *
 *            prime factors of n counted with multiplicity
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 1151, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9539 (2026-11-03)
 * Helpers:   is_prime_u64, next_prime, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/smallest_prime_factor.h"
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

/* exponentiation by squaring: returns base^exp */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* integer ceil k‑th root of N: smallest r such that r^k ≥ N.
 * N ≤ 2^64−1, so we can keep low/high in idx_t.
 */
static idx_t ceil_nth_root(seq_t N, idx_t k)
{
    if (k == 1) return (idx_t)N;
    idx_t low = 1;
    idx_t high = (idx_t)N;               /* N fits into idx_t */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        seq_t mid_pow = pow_seq((seq_t)mid, k);
        if (mid_pow < N)
            low = mid + 1;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* undefined for n<2 */

    /* compute ω(n) */
    idx_t omega = 0;
    idx_t rem = n;
    while (rem > 1) {
        uint64_t p = smallest_prime_factor(rem);
        do {
            rem /= p;
            ++omega;
        } while (rem % p == 0);
    }

    /* ceil root */
    idx_t r = ceil_nth_root((seq_t)n, omega);
    if (r < 2) r = 2;                     /* safety for n=2 */

    /* find least prime ≥ r */
    if (is_prime_big((seq_t)r))
        return (seq_t)r;
    return (seq_t)next_prime(r);
}
