/*
 * A356666  Smallest m such that the m-th Lucas number has exactly n divisors
 *          that are also Lucas numbers.
 * https://oeis.org/A356666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m such that the m‑th Lucas number has exactly n
 *            divisors that are also Lucas numbers. The computation uses the
 *            identity: for m>0 if m = 2^e * o (o odd), D(m) = d(o) (m odd) =
 *            d(o)+1 (m even) and additionally D(m) += 1 iff m%3==0 where d(x)
 *            is the number of divisors of x. The algor...
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 38, u64 n <= 123, u128 n <= 317,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */

/* ---------------------------------------------------------------------- */
/*  Prime lists:   3 is the only prime that is a multiple of 3. */
static const uint64_t primes_with3[] =
{ 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
  53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109,
  113, 127 };
static const size_t primes_with3_len = sizeof(primes_with3) / sizeof(primes_with3[0]);

static const uint64_t primes_without3[] =
{ 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
  53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109,
  113, 127, 131 };
static const size_t primes_without3_len =
        sizeof(primes_without3) / sizeof(primes_without3[0]);

/* ---------------------------------------------------------------------- */
/* DFS that forces inclusion of prime 3 (must take exponent >=1). */
static seq_t best_global;

/* forward declaration */
static void dfs_with3(size_t idx, size_t max_exp, seq_t cur,
                      idx_t cur_div, idx_t target,
                      const uint64_t *primes, size_t primes_len);

/* DFS for the case where 3 is excluded from the factorization */
static void dfs_without3(size_t idx, size_t max_exp, seq_t cur,
                         idx_t cur_div, idx_t target,
                         const uint64_t *primes, size_t primes_len)
{
    if (cur_div == target) {
        if (best_global == (seq_t)0 || cur < best_global)
            best_global = cur;
        return;
    }
    if (idx >= primes_len) return;

    seq_t prime = (seq_t)primes[idx];
    seq_t cur2 = cur;
    for (size_t exp = 1; exp <= max_exp; ++exp) {
        cur2 = cur2 * prime;
        idx_t new_div = cur_div * (exp + 1);
        if (new_div > target) break;
        if (best_global != (seq_t)0 && cur2 >= best_global) break;
        dfs_without3(idx + 1, exp, cur2, new_div, target,
                     primes, primes_len);
    }
}

static void dfs_with3(size_t idx, size_t max_exp, seq_t cur,
                      idx_t cur_div, idx_t target,
                      const uint64_t *primes, size_t primes_len)
{
    if (cur_div == target) {
        if (best_global == (seq_t)0 || cur < best_global)
            best_global = cur;
        return;
    }
    if (idx >= primes_len) return;

    seq_t prime = (seq_t)primes[idx];
    seq_t cur2 = cur;
    /* prime 3 must have exponent >=1 */
    for (size_t exp = 1; exp <= max_exp; ++exp) {
        cur2 = cur2 * prime;
        idx_t new_div = cur_div * (exp + 1);
        if (new_div > target) break;
        if (best_global != (seq_t)0 && cur2 >= best_global) break;
        dfs_with3(idx + 1, exp, cur2, new_div, target,
                  primes, primes_len);
    }
}

/* wrapper: find minimal odd integer with divisor count `target` and
   optionally requiring inclusion of prime 3 */
static seq_t find_min_odd_div(idx_t target, int require_three)
{
    if (target == 0) return (seq_t)0;
    if (require_three) {
        if (target == 1) return (seq_t)0; /* cannot have divisor count 1 and contain 3 */
        best_global = (seq_t)0;
        dfs_with3(0, (size_t)target, (seq_t)1, (idx_t)1, target,
                  primes_with3, primes_with3_len);
        return best_global;
    } else {
        /* 1 always satisfies without an extra 3 factor */
        if (target == 1) return (seq_t)1;
        best_global = (seq_t)0;
        dfs_without3(0, (size_t)target, (seq_t)1, (idx_t)1, target,
                     primes_without3, primes_without3_len);
        return best_global;
    }
}

/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 2) return (seq_t)0;        /* m = 0 is the smallest solution */

    seq_t result = (seq_t)0;
    int found = 0;

    /* 1. Odd m, o%3==0 :  d(o) = n-1   (n>=2) */
    if (n >= 2) {
        seq_t o = find_min_odd_div(n - 1, 1);
        if (o) {
            result = o;
            found = 1;
        }
    }

    /* 2. Odd m, o%3!=0 :  d(o) = n   (n>=1) */
    {
        seq_t o = find_min_odd_div(n, 0);
        if (o) {
            if (!found || o < result) {
                result = o;
                found = 1;
            }
        }
    }

    /* 3. Even m, o%3==0 : d(o) = n-2  (n>=3) */
    if (n >= 3) {
        seq_t o = find_min_odd_div(n - 2, 1);
        if (o) {
            seq_t m = o * (seq_t)2;
            if (!found || m < result) {
                result = m;
                found = 1;
            }
        }
    }

    /* 4. Even m, o%3!=0 : d(o) = n-1  (n>=2) */
    if (n >= 2) {
        seq_t o = find_min_odd_div(n - 1, 0);
        if (o) {
            seq_t m = o * (seq_t)2;
            if (!found || m < result) {
                result = m;
                found = 1;
            }
        }
    }

    return result;    /* results always exist for n>=1 */
}
