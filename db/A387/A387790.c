/*
 * A387790  Number of ways to write n as S(i) + S(j) + 2*S(k), where i,j,k are
 *          positive integers with i >= j, and S(k) is the sum of the first k
 *          primes.
 * https://oeis.org/A387790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 532, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A387790:
 *  a(n) = # of solutions to  S(i)+S(j)+2*S(k) = n
 *  where S(m) = sum of the first m primes,  i >= j >= 1, k >= 1.   */

/*  This implementation builds the list of prefix sums of primes on demand
 *  and counts the admissible triples by a double loop over k and j.
 *  The helper find_sum_index uses a simple linear search;  the number
 *  of needed sums is < 400 for n up to 3·10^5 and is well below the
 *  limits for the required runtimes.  The code is pure C11/C++17 and
 *  uses only integer arithmetic (no floating point).  The result type
 *  seq_t and the index type idx_t are supplied by the build system.   */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- dynamic arrays for primes and cumulative sums ---------- */
static uint64_t   *prime_arr   = NULL;   /* list of primes, 2,3,5,... */
static size_t     prime_cnt   = 0;        /* number of primes stored   */
static size_t     prime_cap   = 0;        /* allocated prime capacity */

static seq_t     *sum_arr    = NULL;      /* S(m) = sum of first m primes   */
static size_t     sum_cnt    = 0;          /* number of sums stored          */
static size_t     sum_cap    = 0;          /* allocated sum capacity          */

/* Extend capacity of an array (used for both prime_arr and sum_arr) */
static void grow_array(void **ptr, size_t *cap, size_t *cnt, size_t elem_size)
{
    if (*cnt >= *cap) {
        size_t new_cap = (*cap == 0) ? 16 : *cap << 1;
        *ptr = realloc(*ptr, new_cap * elem_size);
        *cap   = new_cap;
    }
}

/* Simple deterministic primality test using the current prime list */
static bool is_prime_simple(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_arr[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Append the next prime and its prefix sum to the global arrays */
static void append_next_prime(void)
{
    uint64_t candidate = (prime_cnt == 0) ? 2 : prime_arr[prime_cnt - 1] + 1;
    while (!is_prime_simple(candidate))
        ++candidate;

    /* grow prime array if needed */
    grow_array((void **)&prime_arr, &prime_cap, &prime_cnt,
               sizeof(uint64_t));
    prime_arr[prime_cnt++] = candidate;

    /* compute new cumulative sum */
    seq_t new_sum = (sum_cnt == 0) ?
                        (seq_t)candidate :
                        sum_arr[sum_cnt - 1] + (seq_t)candidate;

    /* grow sum array if needed */
    grow_array((void **)&sum_arr, &sum_cap, &sum_cnt,
               sizeof(seq_t));
    sum_arr[sum_cnt++] = new_sum;
}

/* Ensure that sum_arr contains sums up to at least 'limit' (in seq_t) */
static void extend_sums_to(idx_t limit)
{
    seq_t target = (seq_t)limit;
    while (sum_cnt == 0 || sum_arr[sum_cnt - 1] <= target)
        append_next_prime();
}

/* Linear search for target in sum_arr; returns idx or (idx_t)(-1) */
static idx_t find_sum_index(seq_t target)
{
    if (sum_cnt == 0) return (idx_t)(-1);
    if (target < sum_arr[0]) return (idx_t)(-1);
    if (target > sum_arr[sum_cnt - 1]) return (idx_t)(-1);
    for (size_t i = 0; i < sum_cnt; ++i) {
        if (sum_arr[i] == target) return (idx_t)i;
        if (sum_arr[i] > target) break;
    }
    return (idx_t)(-1);
}

/* ---------- the required function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* out of offset, harmless */

    /* Pre‑compute sums up to at least n (sufficient for all required sums) */
    extend_sums_to(n);

    seq_t count = 0;
    seq_t n_seq = (seq_t)n;

    /* iterate over k : Sk = sum_arr[ki]  (k = ki+1) */
    for (size_t ki = 0; ki < sum_cnt; ++ki) {
        seq_t Sk    = sum_arr[ki];
        seq_t twoSk = (seq_t)2 * Sk;
        if (twoSk > n_seq) break;           /* larger k will only increase twoSk */

        seq_t R = n_seq - twoSk;            /* remaining part: S(i)+S(j) */

        /* iterate over j : Sj = sum_arr[ji]  (j = ji+1) */
        for (size_t ji = 0; ji < sum_cnt; ++ji) {
            seq_t Sj = sum_arr[ji];
            if (Sj > R) break;              /* larger j only increases Sj */

            seq_t target = R - Sj;          /* we need S(i) = target */
            idx_t ii = find_sum_index(target);
            if (ii != (idx_t)(-1) && ii >= (idx_t)ji) {
                /* i >= j (indices are zero‑based) */
                ++count;
            }
        }
    }
    return count;
}
