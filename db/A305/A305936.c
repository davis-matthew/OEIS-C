/*
 * A305936  Irregular triangle whose n-th row is the multiset spanning an
 *          initial interval of positive integers with multiplicities equal to
 *          the n-th row of A296150 (the prime indices of n in weakly
 *          decreasing order).
 * https://oeis.org/A305936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n‑th element of the irregular triangle
   whose m‑th row (m ≥ 2) is constructed from the prime indices of m.
   For m with prime factorization m = Π p_i^{e_i},
   let L contain the indices of the primes p_i, each repeated e_i times,
   sorted in weakly decreasing order.
   The row consists of the numbers 1,2,…,|L|, where the integer j
   appears exactly L[j] times (1‑based).
   The sequence a(n) is the concatenation of these rows, with offset 1
   (a(1) = 1, the first element of the row for m = 2). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* maximum prime stored in the prime table */
static uint64_t *g_primes = NULL;
static size_t g_primes_count = 0;
static uint64_t g_primes_limit = 1;

/* integer square root: floor(sqrt(n)) */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

/* rebuild primes table up to (and including) limit.  If limit ≤ stored
   limit, does nothing.  The table is stored in ascending order in
   g_primes. */
static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= g_primes_limit)
        return;
    if (limit < 2)
        limit = 2;

    size_t sz = limit + 1;
    bool *is_prime = (bool *)malloc(sz);
    if (!is_prime)
        return;       /* out of memory – keep old table */

    for (size_t i = 0; i < sz; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++cnt;

    uint64_t *new_primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_primes) {
        free(is_prime);
        return;
    }

    size_t k = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            new_primes[k++] = i;

    free(is_prime);
    free(g_primes);
    g_primes = new_primes;
    g_primes_count = cnt;
    g_primes_limit = limit;
}

/* binary search for prime index (1‑based) in g_primes */
static idx_t prime_index(uint64_t p)
{
    idx_t lo = 1, hi = g_primes_count;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        uint64_t val = g_primes[mid - 1];
        if (val == p)
            return mid;
        else if (val < p)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return 0; /* should never happen for a prime */
}

/* comparison for qsort: descending order */
static int cmp_desc(const void *a, const void *b)
{
    idx_t ia = *(const idx_t *)a;
    idx_t ib = *(const idx_t *)b;
    if (ia < ib) return 1;
    if (ia > ib) return -1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* unused offset */

    /* start a minimal prime table with first prime 2 */
    ensure_primes_upto(2);

    idx_t cumulative = 0;   /* number of elements processed so far */

    for (idx_t m = 2; ; ++m) {
        /* factor m */
        uint64_t tmp = m;
        idx_t indices[256]; /* list of prime indices */
        idx_t idx_cnt = 0;

        /* make sure we have primes up to sqrt(tmp) */
        idx_t limit = isqrt_u64(tmp);
        if (limit < 2)
            limit = 2;
        ensure_primes_upto(limit);

        /* trial division using precomputed primes */
        for (size_t pi = 0; pi < g_primes_count && (uint64_t)g_primes[pi] * g_primes[pi] <= tmp; ++pi) {
            uint64_t p = g_primes[pi];
            if (tmp % p == 0) {
                idx_t exp = 0;
                while (tmp % p == 0) {
                    tmp /= p;
                    ++exp;
                }
                idx_t idx_val = prime_index(p);
                for (idx_t e = 0; e < exp; ++e)
                    indices[idx_cnt++] = idx_val;
            }
        }

        if (tmp > 1) { /* remaining prime factor */
            ensure_primes_upto(tmp);
            idx_t idx_val = prime_index(tmp);
            indices[idx_cnt++] = idx_val;
        }

        /* sort indices in descending order */
        qsort(indices, idx_cnt, sizeof(idx_t), cmp_desc);

        /* total length of the current row */
        idx_t row_len = 0;
        for (idx_t i = 0; i < idx_cnt; ++i)
            row_len += indices[i];

        if (cumulative + row_len >= n) {
            /* desired element lies in this row */
            idx_t pos = n - cumulative;      /* 1‑based position in the row */
            idx_t acc = 0;
            for (idx_t i = 0; i < idx_cnt; ++i) {
                acc += indices[i];
                if (acc >= pos)
                    return (seq_t)(i + 1);    /* integers start from 1 */
            }
            /* Should not reach here */
            return (seq_t)0;
        }

        cumulative += row_len;
    }
}
