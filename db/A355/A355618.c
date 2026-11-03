/*
 * A355618  a(n) is the least prime that is the sum of a list of numbers > 1
 *          whose product is n, or -1 if there is no such prime.
 * https://oeis.org/A355618
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = least prime that is the sum of a list of numbers > 1
 *            whose product is n, or -1 if no such prime exists
 * Range:     i8 n <= 130, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 92 terms through n = 9101 (2026-11-03)
 * Helpers:   factor_u64, is_prime, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"
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

static void dfs(uint64_t rem, uint64_t last, seq_t sum, seq_t *best)
{
    /* Consider taking rem as the last factor in the factorization. */
    seq_t candidate = sum + (seq_t)rem;
    if (is_prime_big(candidate)) {
        if (*best < 0 || candidate < *best) {
            *best = candidate;
        }
    }

    /* Try further splits with factors >= last. */
    for (uint64_t f = last; f <= rem / f; ++f) {
        if (rem % f == 0) {
            dfs(rem / f, f, sum + (seq_t)f, best);
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)-1;          /* below offset */

    /* 1. If n itself is prime, it is the answer. */
    if (is_prime(n))
        return (seq_t)n;

    /* 2. Sum of prime factorization with multiplicity. */
    uint64_t primes[64];
    uint64_t exps[64];   /* must be uint64_t for factor_u64 */
    size_t   cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum_pf = 0;
    for (size_t i = 0; i < cnt; ++i)
        sum_pf += (seq_t)primes[i] * (seq_t)exps[i];

    if (is_prime_big(sum_pf))
        return sum_pf;

    /* 3. Search all factorizations for the smallest prime sum. */
    seq_t best = -1;
    dfs((uint64_t)n, 2, 0, &best);

    return best;
}
