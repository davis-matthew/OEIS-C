/*
 * A358171  The a(n)-th composition in standard order (A066099) is the first
 *          differences plus one of the prime indices of n (A112798).
 * https://oeis.org/A358171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the index of the composition generated from the multiset
 *            of prime indices of n (A112798) in standard order, A066099
 * Range:     u8 n <= 45, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   build_primes_upto, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/build_spf.h"

static idx_t *spf = NULL;
static idx_t  spf_limit = 0;
static uint64_t *prime_list = NULL;
static size_t  prime_cnt  = 0;
static idx_t  prime_limit = 0;

static void ensure_prime_arrays(idx_t limit)
{
    if (limit > prime_limit) {
        if (prime_list) free(prime_list);
        build_primes_upto(limit, &prime_list, &prime_cnt);
        prime_limit = limit;
    }
}

static void ensure_spf_array(idx_t limit)
{
    if (limit > spf_limit) {
        if (spf) free(spf);
        build_spf(limit, &spf);
        spf_limit = limit;
    }
}

seq_t a(idx_t n)
{
    /* offset is 1; for n<=1 the definition is irrelevant */
    if (n <= 1) return (seq_t)0;

    /* Prepare prime data up to n */
    ensure_prime_arrays(n);
    ensure_spf_array(n);

    /* Factor n and collect prime indices (with multiplicity) */
    idx_t m = 0;                     /* number of indices */
    idx_t idx_buf[64];
    idx_t x = n;
    while (x > 1) {
        idx_t p = spf[x];
        idx_t cnt = 0;
        while (x % p == 0) {
            x /= p;
            ++cnt;
        }

        /* binary search for the index of prime p (1‑based) */
        size_t l = 0, r = prime_cnt;
        while (l < r) {
            size_t mid = (l + r) >> 1;
            if (prime_list[mid] < p) l = mid + 1;
            else r = mid;
        }
        idx_t prime_index = (idx_t)(l + 1);

        for (idx_t i = 0; i < cnt; ++i) {
            idx_buf[m++] = prime_index;
        }
    }

    /* If ≤1 index, the composition is empty: a(n)=0 */
    if (m <= 1) return (seq_t)0;

    /* Compute the composition: first differences + 1 */
    seq_t parts[64];
    idx_t prev = idx_buf[0];
    for (idx_t i = 1; i < m; ++i) {
        seq_t diff = (seq_t)(idx_buf[i] - prev);
        parts[i - 1] = diff + 1;
        prev = idx_buf[i];
    }
    idx_t len = m - 1;

    /* Reconstruct the binary index k from the composition.
       Positions of 1‑bits are the cumulative sums of the reversed parts. */
    seq_t k   = 0;
    seq_t pos = 0;
    for (idx_t i = len; i-- > 0; ) {
        pos += parts[i];
        k += ((seq_t)1) << ( (size_t)(pos - 1) );
    }

    return k;
}
