/*
 * A358089  First differences of A126706.
 * https://oeis.org/A358089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A126706(n+1) – A126706(n) * (first differences of the
 *            sequence of positive integers * that are neither squarefree nor
 *            a prime power). The function * lazily generates the auxiliary
 *            sequence A126706 and caches * terms to support arbitrary call
 *            order
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>


/* Test whether m is not squarefree and not a prime power. */
static int is_candidate_valid(uint64_t m)
{
    uint64_t n = m;
    int distinct = 0;
    int maxexp   = 0;

    /* factor 2 */
    if ((n & 1ULL) == 0) {
        int e = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            ++e;
        }
        ++distinct;
        if (e > maxexp) maxexp = e;
    }

    /* odd prime factors */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            ++distinct;
            if (e > maxexp) maxexp = e;
        }
    }

    /* remaining prime factor (if any) */
    if (n > 1) {
        ++distinct;                       /* exponent is 1 */
        if (maxexp < 1) maxexp = 1;
    }

    return (maxexp >= 2) && (distinct >= 2);
}

/* Cache for A126706 values.  Capacity is ample for n up to ~10 000. */
#define TERMS_CAP 200000
static uint64_t terms[TERMS_CAP];
static size_t terms_len = 0;
static uint64_t next_candidate = 12ULL;   /* first number that can belong to A126706 */

/* Generate A126706 terms until we have at least 'target' terms. */
static void generate_terms(size_t target)
{
    while (terms_len < target && terms_len < TERMS_CAP) {
        if (is_candidate_valid(next_candidate)) {
            terms[terms_len++] = next_candidate;
        }
        ++next_candidate;   /* always move forward */
    }
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* undefined for n < 1 */

    /* ensure terms[0..n] exist */
    generate_terms((size_t)(n + 1));

    /* difference fits in seq_t by definition of the sequence */
    return (seq_t)(terms[n] - terms[n - 1]);
}
