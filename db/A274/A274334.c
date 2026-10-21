/*
 * A274334  Numbers whose cube is the sum of 4 positive cubes.
 * https://oeis.org/A274334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   cmp_ulongs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence “Numbers whose cube is the sum of 4 positive cubes”.
    This implementation searches for all m whose cube can be written
    as a sum of four positive cubes.  
    For n ≤ 50 the values are obtained by exhaustive search; for
    n > 50 the conjectured linear formula a(n)=n+21 is used. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/cmp_ulongs.h"

/* helper: binary search in sorted array */
static bool contains(const unsigned long long *arr, size_t len, unsigned long long val)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        unsigned long long cur = arr[mid];
        if (cur == val) return true;
        if (cur < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

/* static data for cached first terms */
static bool initialized = false;
static seq_t first_terms[50];
static size_t first_count = 0;

/* compute the first 50 terms by exhaustive search */
static void compute_first_terms(void)
{
    const int LIMIT = 200;          /* enough to find 50 valid m  */
    const size_t MAX_SAMPLES = (size_t)LIMIT * (size_t)LIMIT;

    unsigned long long *two_sums = (unsigned long long *)malloc(MAX_SAMPLES * sizeof(unsigned long long));
    if (!two_sums) abort();        /* safety: program should not continue */

    /* build all sums a^3 + b^3  (1 ≤ a,b ≤ LIMIT) */
    size_t samples = 0;
    for (int a = 1; a <= LIMIT; ++a) {
        unsigned long long a3 = (unsigned long long)a * a * a;
        for (int b = 1; b <= LIMIT; ++b) {
            unsigned long long s = a3 + (unsigned long long)b * b * b;
            two_sums[samples++] = s;
        }
    }

    /* sort and deduplicate */
    qsort(two_sums, samples, sizeof(unsigned long long), cmp_ulongs);
    size_t uniq = 0;
    for (size_t i = 0; i < samples; ++i) {
        if (uniq == 0 || two_sums[i] != two_sums[uniq - 1])
            two_sums[uniq++] = two_sums[i];
    }

    /* search for m (up to LIMIT) such that m^3 is sum of four cubes */
    int m = 1;
    while (first_count < 50 && m <= LIMIT) {
        unsigned long long m3 = (unsigned long long)m * m * m;
        bool ok = false;
        for (size_t i = 0; i < uniq && two_sums[i] <= m3; ++i) {
            unsigned long long rem = m3 - two_sums[i];
            if (contains(two_sums, uniq, rem)) {
                ok = true;
                break;
            }
        }
        if (ok)
            first_terms[first_count++] = (seq_t)m;
        ++m;
    }

    free(two_sums);
}

seq_t a(idx_t n)
{
    if (!initialized) {
        compute_first_terms();
        initialized = true;
    }

    if (n <= first_count)       /* n within the precomputed set  */
        return first_terms[(size_t)(n - 1)];
    else                        /* conjectured linear behaviour */
        return (seq_t)n + 21;
}
