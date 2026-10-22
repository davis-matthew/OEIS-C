/*
 * A391982  Irregular triangle read by rows where row n lists the trimmed
 *          zero-based partial alternating sums of the reversed prime indices
 *          of n.
 * https://oeis.org/A391982
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, tabf, changed
 * Formula:   a(n) is the (n‑2)-th element of the triangle whose row r
 *            consists of the trimmed zero‑based partial alternating sums of
 *            the reversed multiset of prime indices of r
 * Range:     i8 n <= 2023, i16 n <= 8582, i32 n <= 8582, i64 n <= 8582,
 *            i128 n <= 8582, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* ------------  Sieve for smallest prime factor  ------------------ */

static idx_t *spf           = NULL;
static uint32_t *prime_index = NULL;
static idx_t  spf_max       = 0;

/* Construct the linear sieve up to 'limit', storing spf[i] for each i
   and the 1–based index of every prime number. */
static void
ensure_spf (idx_t limit)
{
    if (limit <= spf_max) return;

    /* Free previous tables if any */
    free (spf);
    free (prime_index);

    spf           = (idx_t *)calloc (limit + 1, sizeof (idx_t));
    prime_index   = (uint32_t *)calloc (limit + 1, sizeof (uint32_t));

    idx_t *primes   = (idx_t *)malloc (limit * sizeof (idx_t));
    size_t primes_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                     /* i is prime          */
            spf[i] = i;
            primes[primes_cnt++] = i;
            prime_index[i] = (uint32_t)(primes_cnt);  /* 1‑based index */
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            idx_t p   = primes[j];
            idx_t prod = i * p;
            if (prod > limit) break;
            spf[prod] = p;
            if (i % p == 0) break;
        }
    }

    free (primes);
    spf_max = limit;
}

/* ------------  Factorisation utilities  --------------------------- */

static inline size_t
count_factor_len (idx_t n)
{
    size_t cnt = 0;
    idx_t  v   = n;
    while (v > 1) {
        ++cnt;
        v /= spf[v];
    }
    return cnt;
}

/* ------------  Compute the required element of a row  ---------------- */

static seq_t
row_value (idx_t row, size_t pos)
{
    /* Collect the prime indices (with multiplicity) of 'row'.  */
    /* Max possible number of prime factors for n <= 10000
       is less than 32, so a fixed array is sufficient.    */
    static idx_t idx_arr[64];
    size_t len = 0;
    idx_t  v   = row;

    while (v > 1) {
        idx_t p   = spf[v];
        idx_arr[len++] = (idx_t)prime_index[p];
        v /= p;
    }

    /* Compute the partial alternating sums iteratively.
       They satisfy: s_j = -s_{j-1} + reversed_idx_j   (k=0).   */
    seq_t s = 0;
    for (size_t j = 0; j < len; ++j) {
        idx_t rev_idx = idx_arr[len - 1 - j];       /* reversed order */
        s = -s + (seq_t)rev_idx;
        if (j == pos) return s;
    }
    /* Should never reach here for valid inputs. */
    return s;
}

/* ------------  Public sequence function  --------------------------- */

seq_t a (idx_t n)
{
    if (n < 2) return 0;                 /* below the offset */

    idx_t target = n - 2;                 /* 0‑based position in the flattened triangle */

    /* Ensure that spf and prime_index cover the required range. */
    ensure_spf (n);

    idx_t row = 2;
    size_t cumulative = 0;

    for (;; ++row) {
        size_t len = count_factor_len (row);
        if (cumulative + len > target) {
            size_t pos = target - cumulative;
            return row_value (row, pos);
        }
        cumulative += len;
    }
}
