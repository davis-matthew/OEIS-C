/*
 * A352826  Heinz numbers of integer partitions y without a fixed point y(i) =
 *          i. Such a fixed point is unique if it exists.
 * https://oeis.org/A352826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest Heinz number of a partition y * without
 *            a fixed point y(i) = i. The function generates terms by *
 *            scanning integers in increasing order, testing each integer for
 *            * the fixed-point property via its prime factorisation, and *
 *            storing the valid ones in a cache. The cache is...
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
#include "../utils/_floor_isqrt_u64.h"

static seq_t *valid_vals = NULL;      /* sorted sequence of valid Heinz numbers */
static size_t  valid_len  = 0;        /* number of stored terms   */
static size_t  valid_cap  = 0;        /* allocated capacity      */
static idx_t   last_m     = 0;        /* last integer scanned    */

/* dynamic prime table used for factorisation */
static uint64_t *prime_arr = NULL;
static size_t   prime_cnt  = 0;
static idx_t    prime_lim  = 0;        /* largest prime value stored */

static void
ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_lim) return;
    uint64_t *new_arr = NULL;
    size_t   new_cnt = 0;
    build_primes_upto(limit, &new_arr, &new_cnt);
    if (prime_arr) free(prime_arr);
    prime_arr = new_arr;
    prime_cnt = new_cnt;
    prime_lim  = limit;
}

/* append a value to the cache, expanding if necessary */
static void
add_valid(seq_t val)
{
    if (valid_len >= valid_cap) {
        size_t new_cap = (valid_cap == 0) ? 16 : valid_cap * 2;
        seq_t *tmp = (seq_t *)realloc(valid_vals, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        valid_vals = tmp;
        valid_cap  = new_cap;
    }
    valid_vals[valid_len++] = val;
}

/* test whether an integer m has a fixed point in its Heinz-partition */
static bool
is_valid_for_m(idx_t m)
{
    uint64_t num   = (uint64_t)m;
    idx_t    sqrt_m = _floor_isqrt_u64(num);
    ensure_primes_upto(sqrt_m);

    /* arrays holding (prime_index, exponent) pairs */
    int idx_arr[128];
    int exp_arr[128];
    int num_facts = 0;

    for (size_t i = 0; i < prime_cnt && prime_arr[i] <= sqrt_m; ++i) {
        uint64_t p = prime_arr[i];
        if (num % p == 0) {
            int exp = 0;
            while (num % p == 0) {
                ++exp;
                num /= p;
            }
            int idx = (int)(i + 1);        /* prime indices are 1‑based      */
            /* insert in descending order of idx */
            int pos = 0;
            while (pos < num_facts && idx_arr[pos] > idx) ++pos;
            for (int j = num_facts; j > pos; --j) {
                idx_arr[j] = idx_arr[j - 1];
                exp_arr[j] = exp_arr[j - 1];
            }
            idx_arr[pos] = idx;
            exp_arr[pos] = exp;
            ++num_facts;
        }
    }

    if (num > 1) { /* a remaining prime factor > sqrt(m) */
        /* find its index by binary search */
        ensure_primes_upto(num);
        size_t lo = 0, hi = prime_cnt;
        while (lo < hi) {
            size_t mid = (lo + hi) / 2;
            if (prime_arr[mid] < num) lo = mid + 1;
            else if (prime_arr[mid] == num) {
                int idx = (int)(mid + 1);
                int pos = 0;
                while (pos < num_facts && idx_arr[pos] > idx) ++pos;
                for (int j = num_facts; j > pos; --j) {
                    idx_arr[j] = idx_arr[j - 1];
                    exp_arr[j] = exp_arr[j - 1];
                }
                idx_arr[pos] = idx;
                exp_arr[pos] = 1;
                ++num_facts;
                break;
            } else hi = mid;
        }
    }

    /* check for a fixed point in the sorted partition */
    idx_t pos = 0;
    for (int i = 0; i < num_facts; ++i) {
        int idx  = idx_arr[i];
        int exp  = exp_arr[i];
        for (int j = 0; j < exp; ++j) {
            ++pos;
            if (pos == (idx_t)idx) return false;   /* fixed point detected */
        }
    }
    return true;   /* no fixed point */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used per problem statement */

    while (valid_len < (size_t)n) {
        ++last_m;
        if (is_valid_for_m(last_m))
            add_valid((seq_t)last_m);
    }
    return valid_vals[(size_t)(n - 1)];
}
