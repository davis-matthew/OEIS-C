/*
 * A260965  Smallest number r>=3 such that for k>=r the number k - 3 +
 *          prime(n) can be represented in the form x_1*...*x_k + x_1 + ... +
 *          x_k, where 1 <= x_1 <= ... <= x_k, or a(n)=0 if there is no such
 *          r.
 * https://oeis.org/A260965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 125 terms through n = 8981 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = smallest r ≥ 3 such that for k ≥ r the number k – 3 + prime(n)
 * can be written as  x₁·…·x_k + x₁ + … + x_k  with 1 ≤ x₁ ≤ … ≤ x_k,
 * otherwise a(n) = 0.
 *
 * The algorithm searches for the minimal number m (≥ 1) of factors ≥ 2 that
 * satisfy
 *
 *          x₁·…·x_m + x₁ + … + x_m = prime(n) – 3 + m .
 *
 * If such an m exists, the smallest admissible k is max(m,3); the search
 * stops at the first found m (because m increases ⇒ k increases).  If no
 * m up to ⌊log₂ prime(n)⌋ works, the answer is 0.
 *
 * The search for a representation with given m uses a depth‑first search
 * over nondecreasing factor lists; the recursion is pruned aggressively
 * by bounds on product, sum and the target value.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

/* depth‑first search: try to build m factors ≥ start */
static bool dfs(int idx, int m, uint64_t prod, uint64_t sum, int start, uint64_t target)
{
    if (idx == m)
        return prod + sum == target;

    for (int f = start; ; ++f) {
        /* product would overflow target */
        if (prod > target / f) break;
        uint64_t prod2 = prod * (uint64_t)f;
        uint64_t sum2  = sum + f;

        /* total already exceeds the target, further increments only raise it */
        if (prod2 + sum2 > target) break;

        if (dfs(idx + 1, m, prod2, sum2, f, target))
            return true;
    }
    return false;
}

/* wrapper that initiates the DFS */
static bool exists_rep(uint64_t target, int m)
{
    /* minimal product+sum for m factors (all 2) is 2^m + 2m;
       if this bound is already larger than target, no solution exists */
    uint64_t min_prod = (uint64_t)1 << m;
    if (min_prod + (uint64_t)2 * m > target)
        return false;
    return dfs(0, m, 1, 0, 2, target);
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    uint32_t p = get_prime(n);              /* prime(n) fits in 32 bits */
    int max_m = 0;
    uint32_t tmp = p;
    while (tmp > 1) {                       /* compute floor(log2(p)) */
        tmp >>= 1;
        ++max_m;
    }

    for (int m = 1; m <= max_m; ++m) {
        uint64_t target = (uint64_t)p - 3 + m;
        if (exists_rep(target, m)) {
            int k = (m < 3) ? 3 : m;        /* minimal admissible k */
            return (seq_t)k;
        }
    }
    return (seq_t)0;                        /* no representation found */
}
