/*
 * A340959  Table read by antidiagonals of the smallest prime >= n^k, n >= 1
 *          and k >= 0.
 * https://oeis.org/A340959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the smallest prime ≥ N^K where (N,K) are the coordinates
 *            of the n‑th entry read from the table T(N,K) = next_prime(N^K)
 *            antidiagonally, with each diagonal traversed in decreasing N
 *            order (i.e., largest N first)
 * Range:     u8 n <= 32, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-03)
 * Helpers:   ipow, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow.h"
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
    /* Find the diagonal d such that d*(d+1)/2 ≥ n. */
    idx_t d = 1;
    while ( (d * (d + 1)) / 2 < n )
        ++d;

    /* Previous total terms before diagonal d. */
    idx_t prev_total = (d * (d - 1)) / 2;

    /* Zero‑based offset within the diagonal. */
    idx_t offset = n - prev_total - 1;     /* 0 ≤ offset < d */

    /* Coordinates along the diagonal (descending N). */
    seq_t N = (seq_t)(d - offset);        /* N ≥ 1 */
    seq_t K = (seq_t)(offset);            /* K ≥ 0 */

    /* Compute N^K as arbitrary‑precision integer. */
    seq_t power = ipow(N, K);

    /* Find the smallest prime ≥ power. */
    seq_t candidate = power;
    if (candidate <= (seq_t)1)
        candidate = (seq_t)2;
    while (!is_prime_big(candidate))
        ++candidate;
    return candidate;
}
