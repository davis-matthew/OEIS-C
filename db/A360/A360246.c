/*
 * A360246  Numbers for which the prime indices do not have the same mean as
 *          the distinct prime indices.
 * https://oeis.org/A360246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A360246: numbers for which the mean of the multiset of prime
    indices differs from the mean of the distinct prime indices.
    The function returns the nth such number by incremental search with
    memoisation. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *seq = NULL;          /* stored values of the sequence   */
static size_t seq_len = 0;         /* number of terms already computed */
static size_t seq_cap = 0;         /* allocated capacity of seq       */
static uint64_t current = 2;       /* next integer to test           */

/* prime list, built incrementally                           */
static uint64_t *primes = NULL;    /* list of primes in ascending order */
static size_t primes_len = 0;      /* number of primes stored          */
static size_t primes_cap = 0;      /* allocated capacity of primes      */

/* ------------------------------------------------------------------ */
/* Extend the prime list so that it contains all primes <= limit.     */
static void ensure_primes_up_to(uint64_t limit)
{
    if (primes_len != 0 && primes[primes_len-1] >= limit)
        return;

    if (primes_len == 0) {
        /* start with prime 2                                     */
        if (primes_cap == 0) {
            primes_cap = 16;
            primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        }
        primes[primes_len++] = 2;
    }

    uint64_t cand = primes[primes_len-1] + 1;
    if (cand <= 2) cand = 2;
    if (cand % 2 == 0 && cand != 2) cand++; /* next odd candidate   */

    while (primes[primes_len-1] < limit) {
        /* test cand for primality */
        bool isprime = true;
        for (size_t i = 0; i < primes_len &&
             primes[i] * primes[i] <= cand; ++i) {
            if (cand % primes[i] == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            if (primes_len == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(primes,
                                                     new_cap * sizeof(uint64_t));
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = cand;
        }
        cand += 2;   /* next odd */
    }
}

/* ------------------------------------------------------------------ */
/* Return the 1‑based index of prime p (p is guaranteed to be prime). */
static inline size_t prime_index(uint64_t p)
{
    ensure_primes_up_to(p);
    size_t low = 0, high = primes_len;
    while (low < high) {
        size_t mid = (low + high) >> 1;
        if (primes[mid] < p)
            low = mid + 1;
        else
            high = mid;
    }
    /* primes[low] == p expected */
    return low + 1;   /* 1‑based index */
}

/* ------------------------------------------------------------------ */
/* Test whether n satisfies the sequence condition. */
static inline bool passes(uint64_t n)
{
    if (n <= 1) return false;

    uint64_t tmp = n;
    uint64_t sum_mul = 0;
    uint64_t sum_dist = 0;
    uint64_t tot_mul = 0;
    uint64_t distinct = 0;

    /* trial division using the prime list */
    for (size_t i = 0; i < primes_len &&
                 (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp; ++i) {
        if (tmp % primes[i] == 0) {
            uint64_t e = 0;
            while (tmp % primes[i] == 0) {
                tmp /= primes[i];
                ++e;
            }
            size_t idx = prime_index(primes[i]);      /* 1‑based index   */
            sum_mul  += e * idx;
            sum_dist += idx;
            tot_mul  += e;
            ++distinct;
        }
    }

    if (tmp > 1) {                                /* remaining prime  */
        size_t idx = prime_index(tmp);
        sum_mul  += idx;   /* e == 1          */
        sum_dist += idx;
        tot_mul  += 1;
        ++distinct;
    }

    /* compare cross products using 128‑bit to avoid overflow   */
    __int128 lhs = (__int128)sum_mul * distinct;
    __int128 rhs = (__int128)sum_dist * tot_mul;
    return lhs != rhs;
}

/* ------------------------------------------------------------------ */
/* Ensure seq has enough capacity to hold 'needed' elements. */
static inline void ensure_seq_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for 0 */

    /* generate terms until we have at least n terms */
    while (seq_len < n) {
        /* search for next qualifying integer */
        while (true) {
            if (passes(current)) {
                ensure_seq_capacity(seq_len + 1);
                seq[seq_len++] = (seq_t)current;
                ++current;
                break;
            }
            ++current;
        }
    }
    return seq[n - 1];
}
