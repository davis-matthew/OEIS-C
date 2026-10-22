/*
 * A352823  Number of nonfixed points y(i) != i, where y is the weakly
 *          increasing sequence of prime indices of n.
 * https://oeis.org/A352823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonfixed points in the sorted list of prime
 *            indices of n
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Helper: ensure that all primes ≤ limit are stored in the global array.
   The array primes[] holds the primes in increasing order, and primes_cnt
   is the number of primes.  primes_limit is the maximal integer that has
   been sieved so far.  The sieve is re-run only when a larger limit is
   requested. */
static uint32_t *primes = NULL;
static size_t primes_cnt = 0;
static idx_t primes_limit = 0;

static void ensure_primes(idx_t limit)
{
    if (limit <= primes_limit)
        return;

    /* Simple Eratosthenes sieve up to `limit`.  The sieve is small enough
       to recompute each time.  Memory usage is modest even for limits in
       the tens of thousands. */
    bool *is_comp = (bool*)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_comp)
        return;   /* Out of memory – allow the caller to fail gracefully. */

    size_t cap = 64;
    uint32_t *tmp = (uint32_t*)malloc(cap * sizeof(uint32_t));
    size_t tmp_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i)
    {
        if (!is_comp[i])
        {
            if (tmp_cnt == cap)
            {
                cap <<= 1;
                tmp = (uint32_t*)realloc(tmp, cap * sizeof(uint32_t));
                if (!tmp)
                    break;   /* Out of memory – abort early. */
            }
            tmp[tmp_cnt++] = (uint32_t)i;

            if (i <= limit / i)
            {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[j] = true;
            }
        }
    }

    free(is_comp);

    /* Replace the old prime table.  The caller is expected to free `primes`
       if required; here we simply overwrite it. */
    free(primes);
    primes = tmp;
    primes_cnt = tmp_cnt;
    primes_limit = limit;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)   /* Undefined below offset; return 0 by convention. */
        return 0;

    ensure_primes(n);   /* Make sure we have all primes up to n */

    idx_t nn = n;
    seq_t pos = 1;           /* Current position in the sorted multiset (1‑based) */
    seq_t count = 0;         /* Result accumulator */

    for (size_t i = 0; i < primes_cnt; ++i)
    {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > (uint64_t)nn)
            break;

        int exp = 0;
        while (nn % p == 0)
        {
            nn /= p;
            ++exp;
        }
        if (exp > 0)
        {
            seq_t idx = (seq_t)(i + 1);   /* 1‑based index of this prime */
            for (int k = 0; k < exp; ++k)
            {
                if (idx != pos)
                    ++count;
                ++pos;
            }
        }
        if (nn == 1)
            break;
    }

    /* If a prime factor > √(original n) remains, it occurs once. */
    if (nn > 1)
    {
        /* Find the index of the remaining prime. */
        size_t left = 0, right = primes_cnt - 1;
        while (left <= right)
        {
            size_t mid = (left + right) >> 1;
            if (primes[mid] == (uint32_t)nn)
            {
                left = mid;
                break;
            }
            else if (primes[mid] < (uint32_t)nn)
                left = mid + 1;
            else
                right = mid - 1;
        }

        /* In the unlikely event of a bug, guard against out of bounds. */
        if (left >= primes_cnt)
            return count;   /* Something went wrong; return what we have. */

        seq_t idx = (seq_t)(left + 1);
        if (idx != pos)
            ++count;
        ++pos;
    }

    return count;
}
