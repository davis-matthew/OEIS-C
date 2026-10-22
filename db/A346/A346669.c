/*
 * A346669  Numbers r such that the number of nonnegative m < r such that m^k
 *          == m (mod r) is equal to k*(the number of nonnegative m < r such
 *          that -m^k == m (mod r)), where k = 2^A007814(r-1) + 1.
 * https://oeis.org/A346669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   OEIS sequence A346669 – numbers r such that # { m < r | m^k ≡ m
 *            (mod r) } = k * # { m < r | -m^k ≡ m (mod r) } where k =
 *            2^{v2(r-1)} + 1. The implementation tests only odd integers
 *            (even r never satisfies the condition) and uses
 *            prime‑factorisation to evaluate the equality in O(√r) time
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* 2‑adic valuation of a non‑zero 64‑bit integer. */
static inline int tz_uint64(uint64_t x)
{
    int c = 0;
    while ((x & 1ull) == 0) {
        ++c;
        x >>= 1;
    }
    return c;
}

/* Integer square root, floor of √n.  Uses binary search. */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = n;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return low - 1;
}

/* Return true if r satisfies the defining property.  r is assumed odd. */
static inline bool qualifies(uint64_t r)
{
    if (r < 3) return false;            /* no numbers below 3 are accepted */

    uint64_t rminus1 = r - 1;
    int v = tz_uint64(rminus1);         /* v2(r-1) */
    unsigned __int128 kval = ((unsigned __int128)1 << v) + 1;  /* k = 2^v + 1 */

    unsigned __int128 prodA = 1;          /* product of Ap for each prime factor */
    unsigned __int128 prodB = 1;          /* product of Bp for each prime factor */

    uint64_t temp = r;

    /* Factorisation – primes are all odd because r is odd. */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int u = tz_uint64(p - 1);
            int minvu = (v < u) ? v : u;
            unsigned __int128 Ap = ((unsigned __int128)1 << minvu) + 1;
            prodA *= Ap;
            if (v < u) prodB *= kval;          /* Bp = k when v < u */
            while (temp % p == 0) temp /= p;   /* strip the prime power */
        }
    }
    /* Remaining prime factor, if any. */
    if (temp > 1) {
        uint64_t p = temp;
        int u = tz_uint64(p - 1);
        int minvu = (v < u) ? v : u;
        unsigned __int128 Ap = ((unsigned __int128)1 << minvu) + 1;
        prodA *= Ap;
        if (v < u) prodB *= kval;
    }

    unsigned __int128 rhs = kval * prodB;
    return prodA == rhs;
}

/* Storage for previously generated terms. */
static seq_t *seq      = NULL;
static size_t seq_len  = 0;   /* number of terms stored */
static size_t seq_cap  = 0;   /* allocated capacity */
static uint64_t next_candidate = 3;  /* next odd integer to test */

/* Ensure the array has at least 'needed' elements worth of space. */
static void ensure_capacity(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!new_ptr) exit(1);        /* abort on allocation failure */
    seq      = new_ptr;
    seq_cap  = new_cap;
}

seq_t a(idx_t n)
{
    while (seq_len < (size_t)n) {
        /* Test the current candidate. */
        if (qualifies(next_candidate)) {
            ensure_capacity(seq_len + 1);
            seq[seq_len++] = (seq_t)next_candidate;
        }
        /* Advance to the next odd integer. */
        next_candidate += 2;
    }
    return seq[n - 1];
}
