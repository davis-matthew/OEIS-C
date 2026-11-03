/*
 * A374247  The greatest number of runs possible in a permutation of the prime
 *          factors of n (A373957) minus the number of distinct such factors
 *          (A001221).
 * https://oeis.org/A374247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A373957(n) - A001221(n) where A373957(n) is the maximum
 *            number of runs in a permutation of the multiset of prime factors
 *            of n, and A001221(n) is the number of distinct prime factors of
 *            n
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed on the fly by factorising n with
   simple trial division.  The algorithm runs in O(sqrt(n)) time,
   which is easily fast enough for n up to about 10 000. */

#include <stdint.h>
#include <stddef.h>

/* O(N) trial‑division factoriser that also tracks total
   multiplicity, maximal multiplicity, and distinct prime count. */
static seq_t seq_a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    if (nn == 1)          /* a(1) = 0 by definition */
        return 0;

    uint64_t tmp = nn;
    unsigned long long total_cnt = 0;     /* A001222: total prime factors with multiplicity */
    unsigned long long max_cnt   = 0;     /* maximal exponent among primes */
    unsigned long long distinct   = 0;    /* A001221: number of distinct primes */

    /* trial division */
    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if ((tmp % p) == 0) {
            ++distinct;
            unsigned long long exp = 0;
            while ((tmp % p) == 0) {
                tmp /= p;
                ++exp;
            }
            total_cnt += exp;
            if (exp > max_cnt)
                max_cnt = exp;
        }
    }
    /* any remaining prime > sqrt(nn) */
    if (tmp > 1) {
        ++distinct;
        total_cnt += 1;
        if (1 > max_cnt)
            max_cnt = 1;
    }

    /* compute minimal number of adjacent equal pairs required
       (the “merges” in the explanation). */
    unsigned long long merges = 0;
    if (max_cnt > total_cnt - max_cnt + 1)
        merges = max_cnt - (total_cnt - max_cnt + 1);

    /* A373957: maximum runs = total_cnt - merges */
    seq_t max_runs = (seq_t)(total_cnt - merges);

    /* a(n) = max_runs - distinct */
    return max_runs - (seq_t)distinct;
}

/* The required external linkage function */
seq_t a(idx_t n)
{
    return seq_a(n);
}
