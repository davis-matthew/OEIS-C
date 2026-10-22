/*
 * A359041  Number of finite sets of integer partitions with all equal sums
 *          and total sum n.
 * https://oeis.org/A359041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 32, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A359041
    Formula: a(n) = ∑_{d|n} binom(A000041(d), n/d)
    a(n) = Sum_{d|n} binomial(p(d), n/d), where p(d) is the partition
    number A000041(d).  This implementation uses an incremental
    pentagonal‑number recurrence to generate p(d) up to n and computes
    the required binomial coefficients multiplicatively.  All
    arithmetic is performed on the user‑supplied type seq_t; array
    allocation uses new/delete in C++ and malloc/free in C, so the code
    compiles in both C11 and C++17. */

#include <stdint.h>
#include <stdlib.h>

/*--- Globals for partition numbers -------------------------------------*/
static seq_t *parts       = NULL;      /* array of partition numbers */
static idx_t  parts_cap   = 0;         /* allocated capacity of parts  */
static idx_t  parts_len   = 0;         /* number of initialized entries */

/*--- Helpers for dynamic array growth ---------------------------------*/
static void ensure_parts_capacity(idx_t need)
{
    if (need < parts_cap)
        return;

    idx_t new_cap = parts_cap > 0 ? parts_cap : 1;
    while (new_cap <= need)
        new_cap <<= 1;

#ifdef __cplusplus
    seq_t *new_parts = new seq_t[new_cap];
#else
    seq_t *new_parts = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
    for (idx_t i = 0; i < parts_len; ++i)
        new_parts[i] = parts[i];

#ifdef __cplusplus
    delete[] parts;
#else
    free(parts);
#endif
    parts = new_parts;
    parts_cap = new_cap;
}

/*--- Compute partition numbers up to n --------------------------------*/
static void ensure_partitions(idx_t n)
{
    if (parts_len == 0) {
        /* initialise with p(0)=1 */
        ensure_parts_capacity(n);
        parts[0] = (seq_t)1;
        parts_len = 1;
    }

    if (parts_len > n)
        return;                     /* already computed */

    ensure_parts_capacity(n);
    for (idx_t m = parts_len; m <= n; ++m) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m)
                break;
            if (k & 1)
                pos += parts[m - g1];
            else
                neg += parts[m - g1];

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > m)
                continue;
            if (k & 1)
                pos += parts[m - g2];
            else
                neg += parts[m - g2];
        }
        parts[m] = pos - neg;      /* pos ≥ neg for all m */
    }
    parts_len = n + 1;
}

/*--- Multiplicative binomial coefficient C(N,k) -----------------------*/
static seq_t binom(seq_t N, idx_t k)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t term = N - (seq_t)(i - 1);
        res = res * term;
        res = res / (seq_t)i;      /* exact division */
    }
    return res;
}

/*--- Main sequence function -------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    ensure_partitions(n);

    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            seq_t p1 = parts[d1];
            idx_t k1 = n / d1;
            sum += binom(p1, k1);

            if (d1 != d2) {
                seq_t p2 = parts[d2];
                idx_t k2 = n / d2;
                sum += binom(p2, k2);
            }
        }
    }
    return sum;
}
