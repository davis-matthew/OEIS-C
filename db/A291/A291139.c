/*
 * A291139  Primes of the form floor(k^(3/2)) for some integer k.
 * https://oeis.org/A291139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form floor(k^(3/2)) for integer k ≥
 *            2
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 43, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 59 terms through n = 9216 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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
    if (n == 0)
        return 0;   /* undefined for n = 0, safe fallback */

    /* Static state to memoize computed terms */
    static seq_t *seq = NULL;      /* array of found terms */
    static size_t seqcap = 0;      /* allocated capacity of the array */
    static size_t seqlen = 0;      /* number of terms currently stored */
    static idx_t next_k = 2;       /* next integer k to examine */

    /* Generate terms until the n-th term is available */
    while (seqlen < n) {
        /* k^3 = k * k * k, then floor(sqrt(k^3)) = floor(k^(3/2)) */
        seq_t k_seq = (seq_t)next_k;
        seq_t k3 = k_seq * k_seq * k_seq;
        seq_t val = floor_sqrt(k3);

        if (is_prime_big(val)) {   /* test primality */
            /* Grow the array if needed */
            if (seqlen == seqcap) {
                seqcap = seqcap ? seqcap * 2 : 8;
                seq = mem_resize(seq_t, seq, seqcap);
            }
            seq[seqlen++] = val;
        }
        ++next_k;   /* advance to next k */
    }

    return seq[n - 1];
}
