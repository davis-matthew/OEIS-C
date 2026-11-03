/*
 * A395492  Sum of the distinct primes not dividing n that are adjacent to a
 *          divisor of n.
 * https://oeis.org/A395492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9678 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A395492: Sum of the distinct primes not dividing n that are adjacent to a divisor of n. */
#include <stdint.h>
#include <stddef.h>
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

/* Helper: true if p is already stored in seen[0..cnt-1] */
static inline bool contains(uint64_t p, uint64_t *seen, size_t cnt)
{
    for (size_t i = 0; i < cnt; ++i)
        if (seen[i] == p)
            return true;
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1, so 0 is unused */

    uint64_t N     = (uint64_t)n;
    seq_t    sum   = 0;
    /* array of primes already added; 512 is comfortably larger than any
       number of distinct adjacent primes for n up to 10 000 and far beyond
       the limits of typical 64‑bit n. */
    uint64_t seen[512];
    size_t   seen_cnt = 0;

    for (uint64_t d = 1; d <= N / d; ++d) {
        if (N % d != 0) continue;
        /* process divisor d */
        for (int k = -1; k <= 1; k += 2) {   /* k = -1, +1 */
            uint64_t cand = d + (uint64_t)k;
            if (cand < 2) continue;
            if (contains(cand, seen, seen_cnt)) continue;
            if (!is_prime_big((seq_t)cand)) continue;
            if (N % cand == 0) continue;   /* does divide n */
            sum += (seq_t)cand;
            seen[seen_cnt++] = cand;
        }
        /* process the complementary divisor if distinct */
        uint64_t d2 = N / d;
        if (d2 == d) continue;
        for (int k = -1; k <= 1; k += 2) {
            uint64_t cand = d2 + (uint64_t)k;
            if (cand < 2) continue;
            if (contains(cand, seen, seen_cnt)) continue;
            if (!is_prime_big((seq_t)cand)) continue;
            if (N % cand == 0) continue;
            sum += (seq_t)cand;
            seen[seen_cnt++] = cand;
        }
    }
    return sum;
}
