/*
 * A319894  Number of ordered pairs (i,j) with 0 < i < j < prime(n)/2 such
 *          that R(i^4,prime(n)) > R(j^4,prime(n)), where R(k,p) (with p an
 *          odd prime) denotes the unique integer r among 0,...,(p-1)/2 with k
 *          congruent to r or -r modulo p.
 * https://oeis.org/A319894
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (i,j) with 0 < i < j < p/2 such
 *            that R(i^4,p) > R(j^4,p), where p = prime(n) and R(k,p) is the
 *            unique r in [0,(p-1)/2] with k ≡ ±r (mod p)
 * Range:     u8 n <= 20, u16 n <= 173, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 889 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Compute R(i^4,p) where p is an odd prime. */
static inline uint32_t R_of_i4(uint32_t i, uint32_t p, uint32_t half)
{
    /* i^4 mod p using 128‑bit multiplication to avoid overflow */
    uint64_t x = i;
    uint64_t x2 = (unsigned __int128)x * x % p;
    uint64_t x4 = (unsigned __int128)x2 * x2 % p;
    uint32_t r = (uint32_t)x4;
    if (r > half)
        r = p - r;                    /* reflect to [0,half] */
    return r;
}

static uint32_t bit_query(const uint32_t *bit, uint32_t idx)
{
    /* sum of [0 .. idx] inclusive (0‑based) */
    idx++;                           /* shift to 1‑based */
    uint32_t res = 0;
    for (; idx > 0; idx -= idx & -idx)
        res += bit[idx];
    return res;
}

/* Fenwick tree (Binary Indexed Tree) for counting frequencies. */
static void bit_update(uint32_t *bit, uint32_t idx, uint32_t val, uint32_t size)
{
    /* 0‑based idx, tree is 1‑based */
    idx++;                           /* shift to 1‑based */
    for (; idx <= size; idx += idx & -idx)
        bit[idx] += val;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* undefined below offset */

    /* Retrieve the n‑th prime (1‑based). */
    uint32_t p = (uint32_t)get_prime(n);
    uint32_t half = (p - 1) / 2;       /* values range 0..half */

    /* Fenwick tree of size half+1 (values 0..half). */
    uint32_t size = half + 1;
    uint32_t *bit = (uint32_t *)calloc((size_t)size + 1, sizeof(uint32_t));
    if (!bit)
        return 0;                     /* unlikely but safe */

    seq_t total = 0;

    for (uint32_t i = 1; i <= half; ++i) {
        uint32_t r = R_of_i4(i, p, half);       /* 0 .. half */
        uint32_t cnt_le = bit_query(bit, r);    /* elements ≤ r seen so far */
        uint32_t prev = i - 1;
        uint32_t greater = prev - cnt_le;       /* elements > r seen so far */
        total += (seq_t)greater;
        bit_update(bit, r, 1, size);
    }

    free(bit);
    return total;
}
