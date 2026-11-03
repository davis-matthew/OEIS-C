/*
 * A392157  a(n) is the number of prime-to-prime deletion paths from prime(n)
 *          to a one-digit prime, where at each step exactly one decimal digit
 *          is removed and the resulting number, interpreted without leading
 *          zeros, is prime.
 * https://oeis.org/A392157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of prime‑to‑prime deletion paths from prime(n) to
 *            a one‑digit prime. Each step deletes exactly one decimal digit,
 *            removes any leading zeros, and the result must be prime
 * Range:     u8 n <= 666, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 104 terms through n = 9948 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes needed for the utilities and standard helpers */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

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

/* Forward declaration of the recursive routine */
static seq_t dfs_prime(uint64_t p);

/* Static storage for memoisation */
static seq_t *dp = NULL;         /* DP value for each prime number */
static bool  *vis = NULL;        /* Has dp[p] been computed? */
static size_t dp_size = 0;       /* Current allocated size */

/* Ensure the memoisation tables can hold indices up to 'idx' */
static void
ensure_capacity(size_t idx)
{
    if (idx < dp_size)
        return;
    size_t new_size = dp_size ? dp_size : 2;
    while (idx >= new_size)
        new_size <<= 1;          /* double until large enough */

    dp = mem_resize(seq_t, dp, new_size);
    vis = (bool*)realloc(vis, new_size * sizeof(bool));
    /* Initialise the newly added part as not visited */
    memset(vis + dp_size, 0, (new_size - dp_size) * sizeof(bool));
    dp_size = new_size;
}

/* Recursive computation of the number of deletion paths from prime p */
static seq_t
dfs_prime(uint64_t p)
{
    if (p > (size_t)-1)    /* safety: we only support indices that fit in size_t */
        return 0;

    ensure_capacity((size_t)p);

    if (vis[p])
        return dp[p];

    vis[p] = true;

    /* Base case: one‑digit primes terminate with a single valid path */
    if (p < 10) {
        dp[p] = (p == 2 || p == 3 || p == 5 || p == 7) ? 1 : 0;
        return dp[p];
    }

    /* Extract decimal digits into an array (most significant first) */
    uint8_t rev[20];
    int len = 0;
    uint64_t tmp = p;
    while (tmp) {
        rev[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }
    uint8_t msd[20];
    for (int i = 0; i < len; ++i)
        msd[i] = rev[len - 1 - i];   /* msd[0] is most significant */

    seq_t total = 0;
    for (int del = 0; del < len; ++del) {
        uint64_t child = 0;
        for (int j = 0; j < len; ++j) {
            if (j == del) continue;
            child = child * 10 + msd[j];
        }
        if (is_prime_seq((seq_t)child))
            total += dfs_prime(child);
    }

    dp[p] = total;
    return total;
}

/* The required public function */
seq_t
a(idx_t n)
{
    /* prime(n) is given by the utility get_prime */
    uint32_t prime_n = get_prime(n);

    /* Compute the number of deletion paths for this prime */
    return dfs_prime((uint64_t)prime_n);
}
